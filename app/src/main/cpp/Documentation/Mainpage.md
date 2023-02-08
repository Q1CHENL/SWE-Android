/**
 * @file
 * This file is part of SWE.
 *
 * @author Michael Bader (bader AT in.tum.de, http://www5.in.tum.de/wiki/index.php/Univ.-Prof._Dr._Michael_Bader)
 *
 * @section LICENSE
 *
 * SWE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SWE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SWE.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * @section DESCRIPTION
 *
 * Main section of the doxygen documentation.
 */

/** \mainpage SWE mobile - an simple android application 

Tsunami Sim is a android mobile application code which compiles different versions of 
the SWE code. The applications allows for simplified versions of the SWE as well as 
the SWE1D code to be run on an android phone. Additionally, it also provides a simple
F-Wave Solver allowing for the calculation of wave speeds.

\section a simple Shallow Water Code 

SWE is a teaching code that implements simple Finite Volumes models that solve 
the shallow water equations - in a problem setting as it would be used for 
tsunami simulation.  

\section intro The Shallow Water Equations

The shallow water equations describe the behaviour of a fluid, in particular water, 
of a certain (possibly varying) depth <i>h</i> 
in a two-dimensional domain -- imagine, for example, a puddle of water or a shallow pond
(and compare the 1D sketch given below).
The main modelling assumption is that we can neglect effects of flow in vertical 
direction.
The resulting model proved to be useful for the simulation of tsunami propagation
(with appropriate extensions). While an ocean can hardly be considered as 
"shallow" in the usual sense, tsunami waves (in contrast to regular waves induced 
by wind, e.g.) affect the entire water column, such that effects of vertical flow can 
again be neglected. To allow for a non-even sea bottom (as required for accurate 
modelling of tsunamis), we include the elevation <i>b</i> of the sea floor in our model:

The shallow water equations describe the changes of water depth <i>h</i> and 
horizontal velocities <i>v<sub>x</sub></i> and <i>v<sub>y</sub></i> (in the resp.\ coordinate directions) over time, 
depending on some initial conditions -- in the case of tsunami simulation, these initial 
conditions could, for example, result from an initial elevation of the sea floor caused 
by an earthquake. The respective changes in time can be described via a system of 
partial differential equations:

The equation for <i>h</i> is obtained, if we examine the conservation of mass in a control 
volume. The equations for <i>hv<sub>x</sub></i> and <i>hv<sub>y</sub></i> result from conservation of momentum 
(note that <i>h</i> is directly related to the volume, and thus the mass of the water 
-- thus <i>hv<sub>x</sub></i> can be interpreted as a momentum). 

The two terms involving <i>g</i> model a gravity-induced force 
(<i>g</i> being the constant for the gravitational acceleration, <i>g</i> = 9.81 ms<sup>-2</sup>),
which results from the hydrostatic pressure.
The right-hand-side source terms model the effect of an uneven ocean floor
(<i>b</i> obtained from respective bathymetry data).
