#
#####################################################
# Define some global parameters used in the blocks. #
#####################################################
#

[GlobalParams]
###### Other parameters #######
viscosity_name = ENTROPY
isJumpOn = false
Ce = 0.
order = SECOND # CONSTANT # FIRST
family = LAGRANGE
[]

#############################################################################
#                          USER OBJECTS                                     #
#############################################################################
# Define the user object class that store the EOS parameters.               #
#############################################################################

[UserObjects]
active = ' '
  [./JumpGradUS]
    type = JumpGradientInterface
    variable = u
    jump_name = jump_grad_us_aux
    execute_on = timestep_begin
  [../]

[]

###### Mesh #######
[Mesh]
  type = GeneratedMesh
  uniform_refine = 0
  dim = 1
  nx = 64
  xmin = 0
  xmax = 1
  block_id = '0'
  elem_type = EDGE3
[]

#############################################################################
#                             VARIABLES                                     #
#############################################################################
# Define the variables we want to solve for: l=liquid phase and g=gas phase.#
#############################################################################

[Variables]
  [./u]
    scaling = 1e+3
	[./InitialCondition]
        type = ConstantIC # FunctionIC
        value = 0.
        #function = exact_fn
	[../]
  [../]
[]

############################################################################################################
#                                            KERNELS                                                       #
############################################################################################################
# Define the kernels for time dependent, convection and viscosity terms. Same index as for variable block. #
############################################################################################################

[Kernels]

  [./Time]
    type = BadgerTime
    variable = u
  [../]

  [./Flux]
    type = BadgerFlux
    variable = u
  [../]

  [./ViscFlux]
    type = BadgerViscFlux
    variable = u
  [../]

#  [./ForcingTerm]
#    type = BadgerForcingTerm
#    variable = u
#  [../]

  [./ffn]
    type = UserForcingFunction
    variable = u
    function = forcing_fn
  [../]
[]

##############################################################################################
#                                       AUXILARY VARIABLES                                   #
##############################################################################################
# Define the auxilary variables                                                              #
##############################################################################################

[AuxVariables]
#active = ' '
   [./s_aux]
   [../]

   [./us_aux]
   [../]

   [./mu_max_aux]
        family = MONOMIAL
        order = CONSTANT
   [../]

   [./mu_aux]
    family = MONOMIAL
    order = CONSTANT
   [../]

  [./exact_sol_aux]
  [../]

  [./jump_grad_us_aux]
    family = MONOMIAL
    order = CONSTANT
  [../]
[]

##############################################################################################
#                                       AUXILARY KERNELS                                     #
##############################################################################################
# Define the auxilary kernels for liquid and gas phases. Same index as for variable block.   #
##############################################################################################
[AuxKernels]
#active = ' '
  [./EntropyAK]
    type = EntropyAux
    variable = s_aux
    u = u
  [../]

  [./UtimesEntropyAK]
    type = UtimesEntropyAux
    variable = us_aux
    u = u
    s = s_aux
  [../]

  [./MuMaxAK]
    type = MaterialRealAux
    variable = mu_max_aux
    property = mu_max
  [../]

   [./MuAK]
    type = MaterialRealAux
    variable = mu_aux
    property = mu
   [../]

   [./ExactSolution]
    type = FunctionAux
    variable = exact_sol_aux
    function = exact_fn
   [../]

[]

##############################################################################################
#                                       MATERIALS                                            #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################

[Materials]
#active = ' '
  [./EntViscMat]
    type = BadgerComputeViscCoeff
    block = '0'
    u = u
    s = s_aux
    us = us_aux
    #jump_grad_us = jump_grad_us_aux
    PPS_name = AverageEntropy
  [../]

[]

##############################################################################################
#                                     PPS                                                    #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################
[Postprocessors]

  [./AverageEntropy]
    type = NodalMaxValue # ElementAverageValue
    variable = s_aux
    execute_on = timestep_begin
  [../]

  [./l2_err]
    type = ElementL2Error
    variable = u
    function = exact_fn
  [../]
  
  [./l1_err]
    type = ElementL1Error
    variable = u
    function = exact_fn
  [../]
  
  [./h1_err]
    type = ElementH1Error
    variable = u
    function = exact_fn
  [../]

  [./dt]
    type = InviscidTimeStepLimit
    variable = u
    beta = 0.3
  [../]
[]

##############################################################################################
#                                   Functions                                                #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################
[Functions]
  [./forcing_fn]
    type = ParsedFunction
    #value = 4*(t*t*t) # *x-2*(t*t*t)
    #value = exp(t)
    #value = exp(t)*(1-x)*x*(1.+exp(t)*(1.-2*x))
    value = sin(pi*x)*(1.+t*pi*t*cos(pi*x))
    #value = sin(pi*x)*pi*cos(pi*x)
  [../]

  [./exact_fn]
    type = ParsedFunction
    #value=sin(x-t)+2.
    #value=sin(2*pi*x)*t+3.
    value=sin(pi*x)*t
    #value = exp(t)*x*(1.-x)
    #value = sin(pi*x)
  [../]
[]

##############################################################################################
#                               BOUNDARY CONDITIONS                                          #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################
[BCs]
active = 'FunctionDBC'
  [./FunctionDBC]
    type = FunctionDirichletBC
    variable = u
    function = exact_fn
    boundary = '0 1'
  [../]
  
  [./DBC]
    type = DirichletBC
    variable = u
    value = 0.
    boundary = '0 1'
  [../]
  
  [./Periodic]
    [./all]
        variable = u
        auto_direction = 'x'
    [../]
  [../]
[]

##############################################################################################
#                                  PRECONDITIONER                                            #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Preconditioning]
#active = 'FDP_Newton'
   active = 'SMP_Newton'

  [./FDP_Newton]
    type = FDP
    full = true
    solve_type = 'NEWTON'
    line_search = 'default'
#petsc_options = '-snes_mf_operator -snes_ksp_ew'
#petsc_options_iname = '-mat_fd_coloring_err  -mat_fd_type  -mat_mffd_type'
#petsc_options_value = '1.e-12       ds             ds'
  [../]

  [./SMP_Newton]
    type = SMP
    full = true
    solve_type = 'NEWTON'
    #petsc_options = '-ksp_monitor'
    line_search = 'default'
  [../]
[]

##############################################################################################
#                                     EXECUTIONER                                            #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Executioner]
  type = Transient
  scheme = 'implicit-euler' # 'implicit-rk2'
  #rk_scheme = 'sdirk33'
  #end_time = 0.5
# [./TimeStepper]
#   type = FunctionDT
#    time_t =  '0     0.1'
#    time_dt = '2.5e-2  2.5e-2'
#    type = PostprocessorDT
#    postprocessor = dt
#  [../]
  num_steps = 1000.
  dt = 0.0001
  dtmin = 1e-9
  l_tol = 1e-8
  nl_rel_tol = 1e-12
  nl_abs_tol = 1e-10
  l_max_its = 54
  nl_max_its = 10
  [./Quadrature]
    type = GAUSS
    order = TENTH
  [../]
[]
##############################################################################################
#                                        OUTPUT                                              #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Output]
  output_initial = true
  output_displaced = false
  postprocessor_screen = true
  interval = 1
  exodus = true
  perf_log = true
  #linear_residuals = true
[]
