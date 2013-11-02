#include "BadgersBCs.h"

template<>
InputParameters validParams<BadgersBCs>()
{
  InputParameters params = validParams<IntegratedBC>();
    params.addParam<bool>("isImplicit", true, "implicit or explicit schemes.");
  return params;
}

BadgersBCs::BadgersBCs(const std::string & name, InputParameters parameters) :
    IntegratedBC(name, parameters),
    _isImplicit(getParam<bool>("isImplicit")),
    _u_old(valueOld())
{}

Real
BadgersBCs::computeQpResidual()
{
    // Implicit or explicit:
    Real _u_var = (1-(double)_isImplicit)*_u_old[_qp] + (double)_isImplicit*_u[_qp];
    
    // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
    Real _den = _dim == 2 ? 1 : 2;
    RealVectorValue _n(1., (_dim-1)/_den, (_dim-1)*(_dim-2)/_den);
    
    // Compute the flux:
    Real _flux = 0.5*_u_var*_u_var;
    
    // Return value:
    return ( _flux*_test[_i][_qp]*_n*_normals[_qp] );
}

Real
BadgersBCs::computeQpJacobian()
{
    if (_isImplicit) {
        // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
        Real _den = _dim == 2 ? 1 : 2;
        RealVectorValue _n(1., (_dim-1)/_den, (_dim-1)*(_dim-2)/_den);
    
        // Return value:
        return _u[_qp] * _phi[_j][_qp]*(_n*_normals[_qp])*_test[_i][_qp];
    }
    else
        return 0.;
}

Real
BadgersBCs::computeQpOffDiagJacobian(unsigned jvar)
{
    // TODO
    return 0;
}
