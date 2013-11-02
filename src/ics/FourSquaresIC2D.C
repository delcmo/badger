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

#include "FourSquaresIC2D.h"

template<>
InputParameters validParams<FourSquaresIC2D>()
{
    InputParameters params = validParams<InitialCondition>();
    // Input parameters:
    params.addRequiredParam<Real>("upper_left_corner", "Initial value of the upper left corner");
    params.addRequiredParam<Real>("upper_right_corner", "Initial value of the upper right corner");
    params.addRequiredParam<Real>("bottom_left_corner", "Initial value of the bottom left corner");
    params.addRequiredParam<Real>("bottom_right_corner", "Initial value of the bottom right corner");
    // Coordinates of the common node to all squares:
    params.addParam<Real>("x_node", 0.5, "x coord of the common node to all squares.");
    params.addParam<Real>("y_node", 0.5, "y coord of the common node to all squares.");
    // Parameter to smooth the initial discontinuity:
    params.addParam<Real>("length", 0., "length to smooth the data over.");
    return params;
}

FourSquaresIC2D::FourSquaresIC2D(const std::string & name,
                     InputParameters parameters) :
    InitialCondition(name, parameters),
	// Input parameters:
    _upper_left_corner(getParam<Real>("upper_left_corner")),
    _upper_right_corner(getParam<Real>("upper_right_corner")),
    _bottom_left_corner(getParam<Real>("bottom_left_corner")),
    _bottom_right_corner(getParam<Real>("bottom_right_corner")),
    // Coordinate of the common node to all squares:
    _x_node(getParam<Real>("x_node")),
    _y_node(getParam<Real>("y_node")),
    // Length
    _length(getParam<Real>("length"))
{}

Real
FourSquaresIC2D::value(const Point & p)
{
    // If statement on the node coordinates:
    Real _u_ic = 0.;
    
    if (p(0) <= _x_node && p(1) >= _y_node) {
        _u_ic = _upper_left_corner;
    }
    else if (p(0) >= _x_node && p(1) >= _y_node ) {
        _u_ic = _upper_right_corner;
    }
    else if (p(0) <= _x_node && p(1) <= _y_node) {
        _u_ic = _bottom_left_corner;
    }
    else if (p(0) >= _x_node && p(1) <= _y_node) {
        _u_ic = _bottom_right_corner;
    }
    
    // Return the value:
    return _u_ic;
}
