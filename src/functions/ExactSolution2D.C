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

#include "ExactSolution2D.h"

template<>
InputParameters validParams<ExactSolution2D>()
{
  InputParameters params = validParams<Function>();
    params.addParam<Real>("upper_left_corner", "Initial value");
    params.addParam<Real>("upper_right_corner", "Initial value");
    params.addParam<Real>("bottom_left_corner", "Initial value");
    params.addParam<Real>("bottom_right_corner", "Initial value");
  return params;
}

ExactSolution2D::ExactSolution2D(const std::string & name, InputParameters parameters) :
  Function(name, parameters),
    _u_top_left( getParam<Real>("upper_left_corner") ),
    _u_top_right( getParam<Real>("upper_right_corner") ),
    _u_bottom_left( getParam<Real>("bottom_left_corner") ),
    _u_bottom_right( getParam<Real>("bottom_right_corner") )
{
}

ExactSolution2D::~ExactSolution2D()
{
}

Real
ExactSolution2D::value(const Real time, const Point & pt)
{
    // Initialyze some values:
    Real _x1 = 0.5-3.*time/5.;
    Real _x2 = 0.5-time/4.;
    Real _x3 = 0.5+time/2.;
    Real _x4 = 0.5+4.*time/5.;
    Real _y1 = 0.5+3.*time/20;
    Real _y2 = -8.*pt(0)/7.+15./14.-15.*time/28.;
    Real _y3 = pt(0)/6.+5./12.-5.*time/24.;
    Real _y4 = pt(0)-5.*(pt(0)+time-0.5)*(pt(0)+time-0.5)/(18.*time);
    Real _y5 = 0.5-time/10.;
    /*std::cout<<"&&&&&&&&&"<<std::endl;
    std::cout<<"pt(0)="<<pt(0)<<std::endl;
    std::cout<<"pt(1)="<<pt(1)<<std::endl;
    std::cout<<"time="<<time<<std::endl;
    std::cout<<"x1 ="<<_x1<<std::endl;
    std::cout<<"x2 ="<<_x2<<std::endl;
    std::cout<<"x3 ="<<_x3<<std::endl;
    std::cout<<"x4 ="<<_x4<<std::endl;
    std::cout<<"y1 ="<<_y1<<std::endl;
    std::cout<<"y2 ="<<_y2<<std::endl;
    std::cout<<"y3 ="<<_y3<<std::endl;
    std::cout<<"y4 ="<<_y4<<std::endl;
    std::cout<<"y5 ="<<_y5<<std::endl;*/
    
    // If statement:
    if (pt(0) <= _x1) {
        if (pt(1) >= _y1)
            return -0.2;
        else
            return 0.5;
    }
    else if ( pt(0) <= _x2) {
        if (pt(1) >= _y2)
            return -1.;
        else
            return 0.5;
    }
    else if ( pt(0) <= _x3) {
        if (pt(1) >= _y3)
            return -1.;
        else
            return 0.5;
    }
    else if ( pt(0) <= _x4) {
        if (pt(1) >= _y4)
            return -1.;
        else
            return (2.*pt(0)-1.)/(2.*time);
    }
    else {//if (pt(0) >= _x4) {
        if (pt(1) >= _y5)
            return -1.;
        else
            return 0.8;
    }
    //else {
    //    mooseError("The node coordinates do not match with the exact solution (ExactSolution2D.C)");
    //    return 0.;
    //}
}

