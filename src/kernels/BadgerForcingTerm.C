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

#include "BadgerForcingTerm.h"

/**
This Kernel computes the flux for Burger's equations.
*/
template<>
InputParameters validParams<BadgerForcingTerm>()
{
  InputParameters params = validParams<Kernel>();
    params.addParam<bool>("isImplicit", true, "is implicit or not");
  return params;
}

BadgerForcingTerm::BadgerForcingTerm(const std::string & name,
                       InputParameters parameters) :
  Kernel(name, parameters),
    // Implcit boolean:
    _isImplicit(getParam<bool>("isImplicit")),
    // Get material property
    _mu(getMaterialProperty<Real>("mu"))
{}

Real BadgerForcingTerm::computeQpResidual()
{
    // Set time for implicit or explcit scheme:
    Real _time = (double)_isImplicit*_t + (1-(double)_isImplicit)*(_t-_dt);
    //std::cout<<_time<<std::endl;
    
    // Forcing term:
    //std::cout<<_q_point[_qp]<<std::endl;
    Real _cos = std::cos(_q_point[_qp](0)+_q_point[_qp](1)-_time);
    Real _sin = std::sin(_q_point[_qp](0)+_q_point[_qp](1)-_time);
    Real _forcing = _cos*(3.+2*_sin) + 2*_mu[_qp]*_sin;

    // Return the value of the forcing term:
    return -_forcing * _test[_i][_qp];
}

Real BadgerForcingTerm::computeQpJacobian()
{
    return ( 0 );
}

Real BadgerForcingTerm::computeQpOffDiagJacobian( unsigned int _jvar)
{ 
    return ( 0 );
}