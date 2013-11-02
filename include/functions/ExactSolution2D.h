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

#ifndef EXACTSOLUTION2D_H
#define EXACTSOLUTION2D_H

#include "Function.h"

class ExactSolution2D;

template<>
InputParameters validParams<ExactSolution2D>();

class ExactSolution2D : public Function
{
public:
  ExactSolution2D(const std::string & name, InputParameters parameters);
  virtual ~ExactSolution2D();

  virtual Real value(const Real t, const Point & pt);

private:
    Real _u_top_left;
    Real _u_top_right;
    Real _u_bottom_left;
    Real _u_bottom_right;

};

#endif //EXACTSOLUTION2D_H
