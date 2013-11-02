/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "BadgerTime.h"

template<>
InputParameters validParams<BadgerTime>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

BadgerTime::BadgerTime(const std::string & name,
                                             InputParameters parameters) :
    TimeDerivative(name,parameters)
{}

Real
BadgerTime::computeQpResidual()
{
  return TimeDerivative::computeQpResidual();
}

Real
BadgerTime::computeQpJacobian()
{
  return TimeDerivative::computeQpJacobian();
}
