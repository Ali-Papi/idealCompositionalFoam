/*---------------------------------------------------------------------------*\
  		  _______  ____    ____  ________  
 		 |_   __ \|_   \  /   _||_   __  | 
   		   | |__) | |   \/   |    | |_ \_| 
   		   |  ___/  | |\  /| |    |  _|    
    		  _| |_    _| |_\/_| |_  _| |_     
   		 |_____|  |_____||_____||_____|    
   	     Copyright (C) Toulouse INP, Pierre Horgue

License
    This file is part of porousMultiphaseFoam, an extension of OpenFOAM
    developed by Pierre Horgue (phorgue@imft.fr) and dedicated to multiphase 
    flows through porous media.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "porousMediumModel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
defineTypeNameAndDebug(porousMediumModel, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::porousMediumModel::porousMediumModel
(
    const fvMesh& mesh,
    const IOdictionary& transportProperties,
    const word porousRegion
)
    :
    mesh_(mesh),
    transportProperties_(transportProperties),
    eps_
    (
        IOobject
        (
            "eps"+porousRegion,
            mesh.time().constant(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        transportProperties.lookupOrDefault("eps"+porousRegion, dimensionedScalar(dimless, 2))
    ),
    K_
    (
        IOobject
        (
            "K"+porousRegion,
            mesh.time().constant(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        transportProperties.lookupOrDefault("K"+porousRegion, dimensionedScalar(dimArea, 0))
    ),
    sourceTerm_
    (
        IOobject
        (
            "sourceTerm",
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("",dimless/dimTime,0)
    ),
    exchangeTerm_
    (
        IOobject
        (
            "exchangeTerm",
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("",dimless/dimTime,0)
    )
{
    scalar Kfactor(transportProperties.lookupOrDefault<scalar>("Kfactor",1));
    if (Kfactor != 1)
    {
        K_ *= Kfactor;
        Info  << nl << "Reading permeability field factor : Kfactor = " << Kfactor << endl;
    }
}

void Foam::porousMediumModel::check_eps() const
{
    if (gMax(eps_) > 1)
    {
        FatalErrorIn("porousMediumModel.C") <<
            "Field " << eps_.name() << " seems to be uninitialized. You should specify value in transportProperties or field in constant/" << abort(FatalError);
    }
}

void Foam::porousMediumModel::check_K() const
{
    if (gMax(K_) == 0)
    {
        FatalErrorIn("porousMediumModel.C") <<
            "Field " << K_.name() << " is equal to zero. You should specify value in transportProperties or field in constant/" << abort(FatalError);
    }
}

// ************************************************************************* //
