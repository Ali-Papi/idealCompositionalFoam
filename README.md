
# 📌 idealCompositionalFoam Repository Documentation

[![DOI]](https://events.interpore.org/event/56/attachments/1242/2271/InterPore2025%20Book%20of%20Abstracts.pdf#page=987)

# idealCompositionalFoam for OpenFOAM
About
-----

This repository introduces idealCompositionalFoam by [`Papi et al. (2025)`](https://events.interpore.org/event/56/attachments/1242/2271/InterPore2025%20Book%20of%20Abstracts.pdf#page=987) which is an update to [`compositionalIGFoam`](https://github.com/Ali-Papi/compositionalIGFoam) by [`Papi et al. (2024)`](https://www.mdpi.com/1996-1073/17/14/3401). The CO2 / water interactions in a CO2 storage process is simulated in these solvers.
In the last update, the fluids were assumed to be at equilibrium conditions. However, in this update, the equilibrium assumption has been eased. An iterative approach has been adopted in this version which comprises of a transport stage followed by an equilibrium stage. Initially, the species concentrations are calculated at the transport stage then the simulation proceeds to the equilibrium stage at the same timestep to assess the equilibrium condition. A flash calculation is conducted, if the two phases are at equilibrium state, the species concentrations are updated based on the equilibrium condition. So this update not only incorporates the solution of species transport equations but also features a checkpoint that allows for assessing the equilibrium condition.

## Origin

This code is based on porousMultiphaseFoam(PMF), originally developed by Horgue et al. (2015).

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

- then in the "idealCompositionalFoam" directory, run :

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

- [idealCompositionalFoam: An extension to the Darcy-scale compositional solver in OpenFOAM](https://events.interpore.org/event/56/attachments/1242/2271/InterPore2025%20Book%20of%20Abstracts.pdf#page=987)
  Ali Papi, Amir Jahanbakhsh, Gabriel Marcos Magalhaes, Mercedes M. Maroto-Valer;
  *InterPore Conference, Albuquerque US, 19-22 May*, **2025**.
