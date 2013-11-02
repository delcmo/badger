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

#include "BadgerViscFlux.h"

/**
This Kernel computes the flux for Burger's equations.
*/
template<>
InputParameters validParams<BadgerViscFlux>()
{
  InputParameters params = validParams<Kernel>();
    params.addParam<bool>("isImplicit", true, "is implicit or not");
  return params;
}

BadgerViscFlux::BadgerViscFlux(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
    // Implcit boolean:
    _isImplicit(getParam<bool>("isImplicit")),
    // Old value:
    _grad_u_old(_isImplicit ? _grad_zero : gradientOld()),
    // Get material property
    _mu(getMaterialProperty<Real>("mu"))
{}

Real BadgerViscFlux::computeQpResidual()
{
    // Implicit or explicit scheme:
    RealVectorValue _grad_u_var = (1-(double)_isImplicit)*_grad_u_old[_qp] + (double)_isImplicit*_grad_u[_qp];

    // Return the value of the viscsous flux:
    return _mu[_qp] * _grad_u_var * _grad_test[_i][_qp];
}

Real BadgerViscFlux::computeQpJacobian()
{
    if (_isImplicit) {
        // Return the value of the jacobian:
        return _mu[_qp]*_grad_phi[_j][_qp]*_grad_test[_i][_qp];
    }
    else
        return 0.;
}

Real BadgerViscFlux::computeQpOffDiagJacobian( unsigned int _jvar)
{ 
    return ( 0 );
}