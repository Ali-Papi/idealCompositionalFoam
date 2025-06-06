{
    volScalarField potential
        (
            IOobject
            (
                "potential",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );
 
    //-- Reading patch information
    word patchName = args.option("patch");
    label patchID = mesh.boundaryMesh().findPatchID(patchName);
    fvPatchScalarField& potentialPatch = potential.boundaryFieldRef()[patchID];

    //-- Compute and set up head pressure
    if (args.optionFound("DEM"))
    {
        //- reading DEM informations
        DEMfile fixedPotentialDEM(args.option("DEM"));
 	scalar offsetPotential = args.optionLookupOrDefault<scalar>("offset",0.);

        //- reading DEM informations
	if (offsetPotential==0)
	{
	    Info << nl << "Potential fixed using DEM" << endl;
	}
	else
	{
	    Info << nl << "Potential fixed using DEM offseted by " << offsetPotential << endl;
	}

        //- computing local potential
        const vectorField& faces = mesh.boundary()[patchID].patch().faceCentres();
        forAll(potentialPatch,facei)
        {
            potentialPatch[facei] = fixedPotentialDEM.interpolate(faces[facei]) + offsetPotential;
        }
    }
    else if (args.optionFound("STL"))
    {
        //- reading STL informations
        word STLfile = args.option("STL");
        triSurfaceMesh potentialSTL(IOobject(STLfile,mesh));
        pointField pPoints(potentialSTL.points());
	scalar thresholdPotential = args.optionLookupOrDefault<scalar>("threshold",0.);
	scalar offsetPotential = args.optionLookupOrDefault<scalar>("offset",0.);

        Info << nl << "Potential fixed using points over " << thresholdPotential <<  " in STL = " << STLfile << " offseted by " << offsetPotential << endl;
        const vectorField& faces = mesh.boundary()[patchID].patch().faceCentres();
	
        //- computing local potential
        forAll(potentialPatch,facei)
        {
            scalar xy_distance = GREAT;
            label id_point = -1;
            forAll(pPoints,pointi)
            {
                if (pPoints[pointi].z()>thresholdPotential)
		{
                    scalar tmp_dist = Foam::sqrt(pow(pPoints[pointi].x()-faces[facei].x(),2)+pow(pPoints[pointi].y()-faces[facei].y(),2));
                    if (tmp_dist < xy_distance)
		    {
                        xy_distance = tmp_dist;
                        id_point = pointi;
		    }
		}
            }
            potentialPatch[facei] = pPoints[id_point].z() + offsetPotential;    
        }
    }  
    else if (args.optionFound("value"))
    {
        //- uniform potential
        scalar uniformPotential = args.optionLookupOrDefault<scalar>("value",0.);
        Info << nl << "Uniform potential fixed = " << uniformPotential << " m " << endl;
        //- computing and writing local potential
        forAll(potentialPatch,facei)
        {
            potentialPatch[facei] = uniformPotential;
        }
    }

    potential.write();
}
