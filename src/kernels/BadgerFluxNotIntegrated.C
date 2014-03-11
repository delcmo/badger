/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                               */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "BadgerFluxNotIntegrated.h"

/**
This Kernel computes the flux for Burger's equations.
*/
template<>
InputParameters validParams<BadgerFluxNotIntegrated>()
{
  InputParameters params = validParams<Kernel>();
    params.addParam<bool>("isImplicit", true, "is the temporal integrator scheme is implicit?");
  return params;
}

BadgerFluxNotIntegrated::BadgerFluxNotIntegrated(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
    _isImplicit(getParam<bool>("isImplicit")),
    _u_old(_isImplicit ? _zero : valueOld()),
    _grad_u_old(_isImplicit ? _grad_zero : gradientOld())
{}

Real BadgerFluxNotIntegrated::computeQpResidual()
{
    // Implicit or explicit scheme:
    Real _u_var = (1-(double)_isImplicit)*_u_old[_qp] + (double)_isImplicit*_u[_qp];
    RealVectorValue _grad_u_var = (1-(double)_isImplicit)*_grad_u_old[_qp] + (double)_isImplicit*_grad_u[_qp];
    
    // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
    Real _den = _mesh.dimension() == 2 ? 1 : 2;
    RealVectorValue _n(1., (_mesh.dimension()-1)/_den, (_mesh.dimension()-1)*(_mesh.dimension()-2)/_den);
    //std::cout<<_n<<std::endl;
    
    // Compute the flux:
    RealVectorValue _flux = _u_var * _grad_u_var;
    
    // Return the value of the flux:
    return _flux * (_n * _test[_i][_qp]);
}

Real BadgerFluxNotIntegrated::computeQpJacobian()
{
    if (_isImplicit) {
        // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
        Real _den = _mesh.dimension() == 2 ? 1 : 2;
        RealVectorValue _n(1., (_mesh.dimension()-1)/_den, (_mesh.dimension()-1)*(_mesh.dimension()-2)/_den);
    
        // Return the value of the jacobian:
        return ( _phi[_j][_qp] * _grad_u[_qp] + _u[_qp] * _grad_phi[_j][_qp] ) * _n * _test[_i][_qp];
    }
    else
        return 0.;
}

Real BadgerFluxNotIntegrated::computeQpOffDiagJacobian( unsigned int _jvar)
{ 
    return ( 0 );
}
