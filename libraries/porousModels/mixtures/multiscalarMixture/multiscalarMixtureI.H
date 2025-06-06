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

#include "zeroGradientFvPatchField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::multiscalarMixture::multiscalarMixture
(
    const dictionary& dict,
    const wordList& speciesNames,
    const fvMesh& mesh,
    const word& phaseName,
    const volScalarField& eps,
    List<sourceEventFile*>* sourceEventFileRegistry,
    const word& sourceEventDtFieldNameOverride,
    const dimensionSet& sourceTermDimFactor,
    const word& porousRegion
)
:
    basicMixture(dict, mesh, phaseName),
    species_(speciesNames),
    Y_(species_.size()),
    R_(speciesNames.size()),
    lambdas_(speciesNames.size()),
    dispersionModels_(speciesNames.size()),
    sourceEvents_(speciesNames.size()),
    sourceTerms_(speciesNames.size()),
    Kd_(speciesNames.size()),
    rs_(speciesNames.size()),
    epsTotal_(speciesNames.size()),
    eps_(eps)
{
    forAll(speciesNames, speciesi)
    {

        Info<< nl << "******** Specie " << speciesNames[speciesi] << porousRegion << " ********" << endl;
        Y_.set
        (
            speciesi,
            new volScalarField
            (
                IOobject
                (
                    IOobject::groupName(species_[speciesi], phaseName),
                    mesh.time().timeName(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh
            )
        );

        dictionary specieDict(dict.optionalSubDict(speciesNames[speciesi]).optionalSubDict(porousRegion));
        dictionary porousTransport(specieDict.subDict("porousTransport"));

        dimensionedScalar Kd(porousTransport.lookupOrDefault("Kd", dimensionedScalar(dimensionSet(-1,3,0,0,0), 0)));
        Kd_[speciesi] = Kd.value();
        dimensionedScalar rs(porousTransport.lookupOrDefault("rs", dimensionedScalar(dimensionSet(1,-3,0,0,0), 0)));
        rs_[speciesi] = rs.value();
        dimensionedScalar epsTotal(porousTransport.lookupOrDefault("epsTotal", dimensionedScalar(dimless, 1)));
        epsTotal_[speciesi] = epsTotal.value();

        lambdas_[speciesi].dimensions().reset(dimensionSet(0,0,-1,0,0));
        lambdas_[speciesi] = porousTransport.lookupOrDefault("lambda", dimensionedScalar(dimensionSet(0,0,-1,0,0), 0));

        if (porousRegion != "")
        {
            Info << nl << "porousTransport " << porousRegion << " parameters" << nl << "{" << endl;
        }
        else
        {
            Info << nl << "porousTransport parameters" << nl << "{" << endl;
        }
        Info << "    " << Kd.name() << " : " << Kd.value() << endl;
        Info << "    " << rs.name() << " : " << rs.value() << endl;
        Info << "    " << epsTotal.name() << " : " << epsTotal.value() << endl;
        Info << "    " << lambdas_[speciesi].name() << " : " << lambdas_[speciesi].value() << endl;
        Info << "}" << endl;

        Info << nl << "Computing initial retard coefficient (saturated): ";

        R_.set
        (
            speciesi,
            new volScalarField
            (
                IOobject
                (
                    Y(speciesi).name() + porousRegion + "_R",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimless,
                // calculatedFvPatchField<scalar>::typeName
                zeroGradientFvPatchField<scalar>::typeName
            )

        );
        R_[speciesi].primitiveFieldRef() = 1 + (1-epsTotal_[speciesi]) * rs_[speciesi] * Kd_[speciesi] / eps_;
        Info << "Min(R) = " << gMin(R_[speciesi]) << " Max(R) = " << gMax(R_[speciesi]) << endl;

        //- creation of dispersion model
        dispersionModels_.set(speciesi, dispersionModel::New("DeffModel", specieDict, mesh));

        //Handle source events

        const dimensionSet dimSourceTerm = Y(speciesi).dimensions()*sourceTermDimFactor/dimTime;

        //- read source event if present
        if(specieDict.found("sourceEventFileTracer"))
        {
            word sourceEventFileName = specieDict.lookupOrDefault<word>("sourceEventFileTracer","");
            sourceEvents_.set(speciesi, new sourceEventFile(sourceEventFileName));

            const word& dtFieldName = 
                sourceEventDtFieldNameOverride.empty() ? Y(speciesi).name() : sourceEventDtFieldNameOverride;

            sourceEvents_.last().setTimeScheme(dtFieldName, mesh);
            sourceEvents_.last().setFieldDimensions(dimSourceTerm);

            //- report found event to caller
            if(sourceEventFileRegistry)
            {
                sourceEventFileRegistry->append(&sourceEvents_.last());
            }
            else
            {
                FatalErrorIn("multiscalarMixtureI.H")
                    << "sourceEventFileTracer used with an incompatible solver"
                    << abort(FatalError);
            }

        }
        else//- otherwise, create a zero source term of the appropiate dimensions
        {
            sourceTerms_.set
            (
                speciesi, 
                new volScalarField
                (
                    IOobject
                    (
                        "zeroSourceTerm",
                        mesh.time().timeName(),
                        mesh,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh,
                    dimensionedScalar("zero", dimSourceTerm, 0)
                )

            );
        }

    } 

}

// Foam::multiscalarMixture::~multiscalarMixture {};

// * * * * * * * * * * * * * Member Functions * * * * * * * * * * * * //

inline const Foam::speciesTable&
Foam::multiscalarMixture::species() const
{
    return species_;
}

inline bool Foam::multiscalarMixture::contains
(
    const word& specieName
) const
{
    return species_.found(specieName);
}

inline Foam::PtrList<Foam::volScalarField>&
Foam::multiscalarMixture::Y()
{
    return Y_;
}

inline const Foam::PtrList<Foam::volScalarField>&
Foam::multiscalarMixture::Y() const
{
    return Y_;
}

inline Foam::volScalarField& Foam::multiscalarMixture::Y(const label i)
{
    return Y_[i];
}

inline const Foam::volScalarField& Foam::multiscalarMixture::Y
(
    const label i
) const
{
    return Y_[i];
}

inline Foam::volScalarField& Foam::multiscalarMixture::Y
(
    const word& specieName
)
{
    return Y_[species_[specieName]];
}

inline const Foam::volScalarField& Foam::multiscalarMixture::Y
(
    const word& specieName
) const
{
    return Y_[species_[specieName]];
}

inline const Foam::volScalarField& Foam::multiscalarMixture::R(const label speciesi) const
{
    return R_[speciesi];
}

inline const Foam::dimensionedScalar& Foam::multiscalarMixture::lambda(const label speciesi) const
{
    return lambdas_[speciesi];
}

inline void Foam::multiscalarMixture::correct
(
    const volVectorField& U,
    const volScalarField& theta
)
{
    forAll(Y(), speciesi)
    {
        dispersionModels_[speciesi].correct(Y(speciesi), U, theta);
        R_[speciesi].primitiveFieldRef() = 1 + (1-epsTotal_[speciesi]) * rs_[speciesi] * Kd_[speciesi] / theta;
        if(sourceEvents_(speciesi))
        {
            sourceTerms_.set(speciesi, sourceEvents_[speciesi].dtValuesAsField());
        }
    }
}

inline void Foam::multiscalarMixture::correct
(
    const volVectorField& U,
    const volScalarField& saturation,
    const volScalarField& eps
)
{

    forAll(Y(), speciesi)
    {
        dispersionModels_[speciesi].correct(Y(speciesi), U, saturation, eps);
        R_[speciesi].primitiveFieldRef() = 1 + (1-epsTotal_[speciesi]) * rs_[speciesi] * Kd_[speciesi] / (eps*saturation);
        if(sourceEvents_(speciesi))
        {
            sourceTerms_.set(speciesi, sourceEvents_[speciesi].dtValuesAsField());
        }
    }
}

inline const Foam::dimensionedScalar& Foam::multiscalarMixture::Dm(const label speciesi) const
{
    return dispersionModels_[speciesi].Dm();
}

inline const Foam::volTensorField& Foam::multiscalarMixture::Deff(const label speciesi) const
{
    return dispersionModels_[speciesi].Deff();
}


inline const Foam::volScalarField& Foam::multiscalarMixture::sourceTerm(const label speciesi) const
{
    return sourceTerms_[speciesi];
}

// ************************************************************************* //
