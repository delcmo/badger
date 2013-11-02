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

#ifndef FourSquaresIC2D_H
#define FourSquaresIC2D_H

// MOOSE Includes
#include "InitialCondition.h"

// Forward Declarations
class FourSquaresIC2D;

template<>
InputParameters validParams<FourSquaresIC2D>();

class FourSquaresIC2D : public InitialCondition
{
public:

  FourSquaresIC2D(const std::string & name,
            InputParameters parameters);

  virtual Real value(const Point & p);

private:
    // Input parameters:
    const Real & _upper_left_corner;
    const Real & _upper_right_corner;
    const Real & _bottom_left_corner;
    const Real & _bottom_right_corner;
    
    // Coordinate of the common node to all squares:
    const Real & _x_node;
    const Real & _y_node;
    
    // Parameter if want to smooth the initial discontinuity:
    const Real & _length;
    
};

#endif //FourSquaresIC2D_H
