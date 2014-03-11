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
    Real _forcing = 0.;
    if (_mesh.dimension() == 1) {
    /** cos and sin functions for forcing term: */
//        Real _cos = std::cos(libMesh::pi*_q_point[_qp](0));
//        Real _sin = std::sin(libMesh::pi*_q_point[_qp](0));
//        Real _u_val = _sin*_time;
////        Real _u_val = _sin+3.;
//        _forcing = _sin*(1+2.*libMesh::pi*libMesh::pi*_mu[_qp]*_time) + libMesh::pi*_u_val*_cos*_time;
////        _forcing = _sin*4.*libMesh::pi*libMesh::pi*_mu[_qp] + 2.*libMesh::pi*_u_val*_cos;
////        Real _cos = std::cos(2*libMesh::pi*_q_point[_qp](0));
////        Real _sin = std::sin(2*libMesh::pi*_q_point[_qp](0));
    
    /** Gaussian function for forcing term */
        Real exp_val = std::exp( -(_q_point[_qp](0)-_time-0.5)*(_q_point[_qp](0)-_time-0.5) );
        Real u_val = _q_point[_qp](0)*(_q_point[_qp](0)-1.)*exp_val;
        Real dudx = ( (2*_q_point[_qp](0)-1.)-2*_q_point[_qp](0)*(_q_point[_qp](0)-1.)*(_q_point[_qp](0)-_time-0.5) )*exp_val;
        Real dudt = 2*_q_point[_qp](0)*(_q_point[_qp](0)-1.)*(_q_point[_qp](0)-_time-0.5)*exp_val;
        _forcing = dudt+u_val*dudx;
        
    }
    else if (_mesh.dimension() == 2) {
    //    Real _cos = std::cos(_q_point[_qp](0)+_q_point[_qp](1)-_time);
    //    Real _sin = std::sin(_q_point[_qp](0)+_q_point[_qp](1)-_time);
    //    _forcing = _cos*(3.+2*_sin) + 2*_mu[_qp]*_sin;
        
        Real _cosx = std::cos(2*libMesh::pi*_q_point[_qp](0));
        Real _cosy = std::cos(2*libMesh::pi*_q_point[_qp](1));
        Real _sinx = std::sin(2*libMesh::pi*_q_point[_qp](0));
        Real _siny = std::sin(2*libMesh::pi*_q_point[_qp](1));
        Real _u_val = _sinx*_siny*_time+3.;
        _forcing = _sinx*_siny*(1.+8.*libMesh::pi*libMesh::pi*_mu[_qp]*_time)+2.*libMesh::pi*_u_val*(_cosx*_siny+_sinx*_cosy)*_time;
    }
    
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
