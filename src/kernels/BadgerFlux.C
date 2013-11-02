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

#include "BadgerFlux.h"

/**
This Kernel computes the flux for Burger's equations.
*/
template<>
InputParameters validParams<BadgerFlux>()
{
  InputParameters params = validParams<Kernel>();
    params.addParam<bool>("isImplicit", true, "is the temporal integrator scheme is implicit?");
  return params;
}

BadgerFlux::BadgerFlux(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
    _isImplicit(getParam<bool>("isImplicit")),
    _u_old(_isImplicit ? _zero : valueOld())
{}

Real BadgerFlux::computeQpResidual()
{
    // Implicit or explicit scheme:
    Real _u_var = (1-(double)_isImplicit)*_u_old[_qp] + (double)_isImplicit*_u[_qp];
    
    // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
    Real _den = _dim == 2 ? 1 : 2;
    RealVectorValue _n(1., (_dim-1)/_den, (_dim-1)*(_dim-2)/_den);
    //std::cout<<_n<<std::endl;
    
    // Compute the flux:
    Real _flux = 0.5 * _u_var * _u_var;
    
    // Return the value of the flux:
    return -_flux * (_n * _grad_test[_i][_qp]);
}

Real BadgerFlux::computeQpJacobian()
{
    if (_isImplicit) {
        // Set a vector n (in 1D n(1,0,0), in 2D n(1,1,0) and in 3D n(1,1,1)):
        Real _den = _dim == 2 ? 1 : 2;
        RealVectorValue _n(1., (_dim-1)/_den, (_dim-1)*(_dim-2)/_den);
    
        // Return the value of the jacobian:
        return ( -_u[_qp] * _phi[_j][_qp] * (_n * _grad_test[_i][_qp]) );
    }
    else
        return 0.;
}

Real BadgerFlux::computeQpOffDiagJacobian( unsigned int _jvar)
{ 
    return ( 0 );
}
