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

#ifndef EXACTSOLUTION1D_H
#define EXACTSOLUTION1D_H

#include "Function.h"

class ExactSolution1D;

template<>
InputParameters validParams<ExactSolution1D>();

class ExactSolution1D : public Function
{
public:
  ExactSolution1D(const std::string & name, InputParameters parameters);
  virtual ~ExactSolution1D();

  virtual Real value(const Real t, const Point & pt);

private:
};

#endif //ExactSolution1D_H
