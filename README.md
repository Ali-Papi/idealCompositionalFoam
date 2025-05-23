
# 📌 compositionalIGFoam Repository Documentation

[![DOI](https://img.shields.io/badge/DOI-10.3390/en17143401-blue)](https://www.mdpi.com/1996-1073/17/14/3401)
[![DOI](https://img.shields.io/badge/DOI-10.3390/en17143401-blue)](https://events.interpore.org/event/46/contributions/7161)

# CompositionalIGFoam for OpenFOAM
About
-----

This is a solver developed on top of the `PorousMultiphaseFoam(PMF)` project for compositional flow modeling in porous media. It has been developed based on OpenFOAM-10 and includes a new solver (compositionalIGFoam) for H2O / CO2 interactions. The impesFoam solver of the PMF package has been modified to accommodate a compositional solver.
The model developed in this study is the first of its kind in OpenFOAM to address CO2/water interactions in a multi-phase flow at the Darcy scale.

## Origin

This code is based on [porousMultiphaseFoam(PMF)](https://github.com/phorgue/porousMultiphaseFoam), originally developed by Horgue et al. (2015).

**PMF** is an open-source toolbox dedicated to simulation of flow and transport processes in porous media.

It is based on the OpenFOAM environment and therefore benefits from its multiple feature such as pre-and post-processing tools or parallel efficiency for example.


# General Information

- This toolbox needs only a standard OpenFOAM installation and is compatible with the OpenFOAM-10
  (see www.openfoam.org or www.openfoam.com).

- Please cite the related paper in the "doc" folder if you are using this
  toolbox.

- Read the COPYING_OPENFOAM file for information about OpenFOAM and this
  toolbox Copyrights.

# Installation instructions :


- First, source the OpenFOAM configuration file, i.e. (example for ubuntu
  version) :

> source /opt/openfoamv6/etc/bashrc

- then in the "porousMultiphaseFoam" directory, run :

> ./Allwmake -jX

  to install the package (with X the number of processors).

- Dynamic libraries are compiled in the standard OpenFOAM user directory :

> $FOAM_USER_LIBBIN

- The executable solver "impesFoam" is placed in the standard OpenFOAM user
  directory $FOAM_USER_APPBIN.

- Each tutorial directory contains "run" and "clean" files to test installation
  and validate the solver.

- A python script runTutorials.py can be used to test all components.

- To remove compilation and temporary files, run :

> ./Allwclean

- see the ReleaseNotes.txt file for detailed information about the toolbox.

---

How to cite
---------------

If your are using the compositionalIGFoam, please cite as follows:

- [A New Straightforward Darcy-Scale Compositional Solver in OpenFOAM for CO2/Water Mutual Solubility in CO2 Storage Processes in Aquifers](https://www.mdpi.com/1996-1073/17/14/3401)
  A. Papi, A. Jahanbakhsh, M.M. Maroto-Valer;
  *MDPI Energies  17(14), 3401*, **2024**.
