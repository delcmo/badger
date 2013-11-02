#include "BadgerComputeViscCoeff.h"

template<>
InputParameters validParams<BadgerComputeViscCoeff>()
{
  InputParameters params = validParams<Material>();
    // Viscosity type:
    params.addParam<std::string>("viscosity_name", "ENTROPY", "first order or entropy viscosity type.");
    // is implcit:
    params.addParam<bool>("isImplicit", true , "implicit or explicit schemes.");
    // Boolean for jumps:
    params.addParam("isJumpOn", true, "Is jump on?.");
    // Coupled variables:
    params.addRequiredCoupledVar("u", "Variable it is solved for.");
    params.addRequiredCoupledVar("s", "Entropy function.");
    params.addRequiredCoupledVar("us", "variable it is solved for times the entropy function.");
    params.addCoupledVar("jump_grad_us", "jump of us gradient");
    // Cconstant parameter:
    params.addParam<double>("Ce", 1., "Coefficient for viscosity");
    // PPS names:
    params.addParam<std::string>("PPS_name", "none", "name of the pps");
    return params;
}

BadgerComputeViscCoeff::BadgerComputeViscCoeff(const std::string & name, InputParameters parameters) :
    Material(name, parameters),
    // Boolean for implicit:
    _isImplicit(getParam<bool>("isImplicit")),
    // Viscosity type:
    _visc_name(getParam<std::string>("viscosity_name")),
    // Boolean for jumps:
    _isJumpOn(getParam<bool>("isJumpOn")),
    // Declare aux variables:
    _u(_isImplicit ? coupledValue("u") : coupledValueOld("u")),
    _s(_isImplicit ? coupledValue("s") : coupledValueOld("s")),
    _s_old(_isImplicit ? coupledValueOld("s") : coupledValueOlder("s")),
    _grad_us(_isImplicit ? coupledGradient("us") : coupledGradientOld("us")),
    _grad_us_old(_isImplicit ? coupledGradientOld("us") : coupledGradientOlder("us")),
    // Jump of pressure and density gradients:
    _jump_grad_u(isCoupled("jump_grad_us") ? coupledValue("jump_grad_us") : _zero),
    _jump_grad_u_old(isCoupled("jump_grad_us") ? coupledValueOlder("jump_grad_us") : _zero),
    // Declare material properties
    _mu(declareProperty<Real>("mu")),
    //_mu_old(declarePropertyOld<Real>("mu")),
    _mu_max(declareProperty<Real>("mu_max")),
    // Get parameter Ce
    _Ce(getParam<double>("Ce")),
    // PPS name:
    _pps_name(getParam<std::string>("PPS_name"))
{}

void
BadgerComputeViscCoeff::initQpStatefulProperties()
{
    // Determine h (length used in definition of first and second order viscosities):
    Real _h = _current_elem->hmin() / _qrule->get_order();
    
    // Return the value of mu_old for the first time step:
    //_mu[_qp] = 0.5 * _h * std::fabs(_u[_qp]);
    //_mu_old[_qp] = _mu[_qp];
}

void
BadgerComputeViscCoeff::computeQpProperties()
{
    // Determine h (length used in definition of first and second order viscosities):
    Real _h = _current_elem->hmin() / _qrule->get_order();
    //std::cout<<"h="<<_h<<std::endl;
    
    // Epsilon value normalization of unit vectors:
    Real _eps = std::sqrt(std::numeric_limits<Real>::min());
    
    // Get the pps value for pressure and velocity:
    Real _pps = _isImplicit ? std::max(getPostprocessorValueByName(_pps_name), _eps) : std::max(getPostprocessorValueOldByName(_pps_name), _eps);
    //std::cout<<_pps<<std::endl;
    
    // Compute the first order viscosity:
    _mu_max[_qp] = 0.5 * _h * std::fabs(_u[_qp]);
    //std::cout<<"mu max="<<_mu_max[_qp]<<std::endl;
    
    // Compute the jump:
    Real _jump = (double)_isJumpOn * ( _isImplicit ? _jump_grad_u[_qp] : _jump_grad_u_old[_qp]);
    //std::cout<<"bool="<<(double)_isJumpOn<<std::endl;
    //std::cout<<"jump="<<_jump<<std::endl;
    
    // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
    Real _den = _dim == 2 ? 1 : 2;
    RealVectorValue _n(1., (_dim-1)/_den, (_dim-1)*(_dim-2)/_den);
    
    // Compute the entropy residual (CN):
    Real _residual = std::fabs( (_s[_qp] - _s_old[_qp]) / _dt + 0.5*_n*(_grad_us[_qp] + _grad_us_old[_qp])*2/3 );
    //std::cout<<"residual="<<_residual<<std::endl;
    //std::cout<<"pps="<<_pps<<std::endl;
    //std::cout<<"_s="<<_s[_qp]<<std::endl;
    
    // Determine the max between jump and entropy residual, and normalize:
    //Real _resid_jump = std::max(_residual, _jump) / _pps;
    Real _resid_jump = (_residual + _jump) / _pps;
    //std::cout<<"resid_jump="<<_resid_jump<<std::endl;
    
    // Return the value of the viscosity:
    _mu[_qp] =  std::min(_mu_max[_qp], _Ce*_h*_h*_resid_jump);
    //std::cout<<"min="<<std::min(_mu_max[_qp], _resid_jump)<<std::endl;
    //std::cout<<"mu="<<_mu[_qp]<<std::endl;
    //std::cout<<"ce="<<_Ce<<std::endl;
    //std::cout<<"Ce*h^2="<<_Ce*_h*_h<<std::endl;
}