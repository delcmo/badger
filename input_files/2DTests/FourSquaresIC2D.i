#
#####################################################
# Define some global parameters used in the blocks. #
#####################################################
#

[GlobalParams]
###### Other parameters #######
order = FIRST
viscosity_name = ENTROPY
isJumpOn = true
Ce = 1.

###### ICs ##########
upper_left_corner = -0.2
upper_right_corner = -1.
bottom_left_corner = 0.5
bottom_right_corner = 0.8
x_node = 0.5
y_node = 0.5
[]

#############################################################################
#                          USER OBJECTS                                     #
#############################################################################
# Define the user object class that store the EOS parameters.               #
#############################################################################

[UserObjects]

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
  dim = 2
  nx = 25
  ny = 25
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  block_id = '0'
[]

#############################################################################
#                             VARIABLES                                     #
#############################################################################
# Define the variables we want to solve for: l=liquid phase and g=gas phase.#
#############################################################################

[Variables]
  [./u]
    family = LAGRANGE
    scaling = 1e+3
	[./InitialCondition]
        type = FourSquaresIC2D
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
[]

##############################################################################################
#                                       AUXILARY VARIABLES                                   #
##############################################################################################
# Define the auxilary variables                                                              #
##############################################################################################

[AuxVariables]
   [./s_aux]
      family = LAGRANGE
   [../]

   [./us_aux]
      family = LAGRANGE
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
    family = LAGRANGE
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

   [./ExactSolutionAK]
    type = FunctionAux
    variable = exact_sol_aux
    function = ExactSolution
   [../]

[]

##############################################################################################
#                                       MATERIALS                                            #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################

[Materials]
#active = ''
  [./EntViscMat]
    type = BadgerComputeViscCoeff
    block = '0'
    u = u
    s = s_aux
    us = us_aux
    jump_grad_us = jump_grad_us_aux
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
    type = ElementAverageValue
    variable = s_aux
    execute_on = timestep_begin
  [../]

  [./L2ElementError]
    type = ElementL2Error
    variable = u
    function = ExactSolution
#execute_on = timestep_begin
  [../]

  [./L1ElementError]
    type = ElementL1Error
    variable = u
    function = ExactSolution
    #execute_on = timestep_begin
  [../]

  [./L2NodalError]
    type = NodalL2Error
    variable = u
    function = ExactSolution
    #execute_on = timestep_begin
  [../]

  [./H1norm]
    type = ElementH1Error
    variable = u
    function = ExactSolution
    #execute_on = timestep_begin
  [../]
[]

##############################################################################################
#                                   Functions                                                #
##############################################################################################
# Define functions that are used in the kernels and aux. kernels.                            #
##############################################################################################
[Functions]
  [./ExactSolution]
    type = ExactSolution2D
  [../]
[]

##############################################################################################
#                               BOUNDARY CONDITIONS                                          #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################
[BCs]
#active = ' '
  [./RightDBC]
    type = BadgersBCs
    variable = u
    boundary = 'left'
  [../]

  [./LeftDBC]
    type = BadgersBCs
    variable = u
    boundary = 'right'
  [../]

  [./TopBC]
    type = BadgersBCs
    variable = u
    boundary = 'top'
  [../]

  [./BottomBC]
    type = BadgersBCs
    variable = u
    boundary = 'bottom'
  [../]
[]

##############################################################################################
#                                  PRECONDITIONER                                            #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Preconditioning]
#  active = 'FDP_Newton'
  active = 'SMP_Newton'

  [./FDP_Newton]
    type = FDP
    full = true
    petsc_options = '-snes_mf_operator -snes_ksp_ew'
    petsc_options_iname = '-mat_fd_coloring_err  -mat_fd_type  -mat_mffd_type'
    petsc_options_value = '1.e-12       ds             ds'
  [../]

  [./SMP_Newton]
    type = SMP
    full = true
    slve_type = 'PJFNK'
  [../]
[]

##############################################################################################
#                                     EXECUTIONER                                            #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Executioner]
  type = Transient
  string scheme = 'bdf2'
  end_time = 0.5
  #num_steps = 10
  dt = 5.e-3
  dtmin = 1e-9
  l_tol = 1e-8
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-5
  l_max_its = 54
  nl_max_its = 30
  [./Quadrature]
    type = TRAP
  [../]
#  [./TimeStepper]
#    type = FunctionDT
#    time_t =  '0    1e-4  2e-4  0.5'
#    time_dt = '1e-4 1e-4  1e-2  1e-2'
#  [../]
[]

#[Adaptivity]
#  initial_marker = errorfrac
#  marker = errorfrac
#  max_h_level = 5
#  [./Indicators]
#    [./error]
#        type = GradientJumpIndicator
#        #type = FluxJumpIndicator
#        #type = LaplacianJumpIndicator
#        variable = u
#        block = '0'
#    [../]
#  [../]
#  [./Markers]
#    block = '0'
#    [./errorfrac]
#type = ErrorFractionMarker
#        type = ErrorToleranceMarker
#        refine = 0.5
#        coarsen = 0.01
#        indicator = error
#    [../]
#  [../]
# [../]
[]
##############################################################################################
#                                        OUTPUT                                              #
##############################################################################################
# Define the functions computing the inflow and outflow boundary conditions.                 #
##############################################################################################

[Output]
  output_initial = true
  output_displaced = false
  #file_base = FourSquaresIC2D
  postprocessor_screen = false
  interval = 1
  exodus = true
  perf_log = true
[]
