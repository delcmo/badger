#include "BadgerApp.h"
#include "Moose.h"
#include "ElkApp.h"
#include "AppFactory.h"
// Kernels
#include "BadgerTime.h"
#include "BadgerFlux.h"
#include "BadgerFluxNotIntegrated.h"
#include "BadgerViscFlux.h"
#include "BadgerForcingTerm.h"
// Auxkernels
#include "EntropyAux.h"
#include "UtimesEntropyAux.h"
// Materials
#include "BadgerComputeViscCoeff.h"
// Userobjects
#include "JumpGradientInterface.h"
#include "SmoothingFunction.h"
// BCs
#include "BadgersBCs.h"
// ICS
#include "FourSquaresIC2D.h"
// Functions
#include "ExactSolution2D.h"
#include "ExactSolution1D.h"
// Postprocessors
#include "ElementL1Error.h"
#include "InviscidTimeStepLimit.h"

template<>
InputParameters validParams<BadgerApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

BadgerApp::BadgerApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());
  
  Moose::registerObjects(_factory);
  ElkApp::registerObjects(_factory);
  BadgerApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ElkApp::associateSyntax(_syntax, _action_factory);
  BadgerApp::associateSyntax(_syntax, _action_factory);
}

BadgerApp::~BadgerApp()
{
}

void
BadgerApp::registerApps()
{
  registerApp(BadgerApp);
}

void
BadgerApp::registerObjects(Factory & factory)
{
    // Kernels:
    registerKernel(BadgerTime);
    registerKernel(BadgerFlux);
    registerKernel(BadgerFluxNotIntegrated);
    registerKernel(BadgerViscFlux);
    registerKernel(BadgerForcingTerm);
    // Auxkernels
    registerAux(EntropyAux);
    registerAux(UtimesEntropyAux);
    // Materials
    registerMaterial(BadgerComputeViscCoeff);
    // Userobjects
    registerUserObject(JumpGradientInterface);
    registerUserObject(SmoothingFunction);
    // BCs
    registerBoundaryCondition(BadgersBCs);
    // ICs
    registerInitialCondition(FourSquaresIC2D);
    // Functions
    registerFunction(ExactSolution2D);
    registerFunction(ExactSolution1D);
    // Postprocessors:
    registerPostprocessor(ElementL1Error);
    registerPostprocessor(InviscidTimeStepLimit);
}

void
BadgerApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
