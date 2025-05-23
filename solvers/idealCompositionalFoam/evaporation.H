{
    NH2O = eps*xH2O*rhob*Sb/(Mw) + eps*yH2O*rhoa*(1-Sb)/(Mg);
    NCO2 = eps*xCO2*rhob*Sb/(Mw) + eps*yCO2*rhoa*(1-Sb)/(Mg);
    NT = eps*rhob*Sb/(Mw) + eps*rhoa*(1-Sb)/(Mg);
    //NCO2g = eps*yCO2*rhoa*(1-Sb)/(Mg);
    NCO2l = eps*xCO2*rhob*Sb/(Mw);
    //NH2Ol = eps*xH2O*rhob*Sb/(Mw);
    NH2Og = eps*yH2O*rhoa*(1-Sb)/(Mg);
    NL = eps*rhob*Sb/(Mw);
    NCO2g = yCO2*(NT-NL);
    NH2Ol = xH2O*NL;
    NG = NT-NL;
    volScalarField sigma1 = NCO2/(NT*PH) + NH2O/(NT*PP) ;
    volScalarField sigma2 = NCO2*PH/NT + NH2O*PP/NT ;
    volScalarField NLCO2min = NL;
    volScalarField NCO2_t = NCO2l+ NCO2g;
    NCO2g_t = NCO2g;
    NH2Ol_t = NH2Ol;


    // -------------------------------- method 2 ---------------------------   
    
    forAll(NCO2g, cellI)
    {
	    if (NCO2g[cellI]> 0)
	    {
		NLflash[cellI] = (NH2O[cellI])/(1-PH[cellI]) - (NCO2[cellI])/(PP[cellI]-1);
		
		if (NLflash[cellI] < NLmax[cellI] || NLflash[cellI] < NT[cellI]) // || sigma2[cellI] < 1
		// || sigma1[cellI] >1)
		{
			NL[cellI]  = NLflash[cellI];
			yCO2[cellI] = (PP[cellI]-1)/(PP[cellI]-PH[cellI]);
			xCO2[cellI] = yCO2[cellI]*PH[cellI];
			yH2O[cellI]= 1- yCO2[cellI];
			xH2O[cellI]= 1- xCO2[cellI];
		}
		
		//Sb[cellI] = (NL[cellI]*Mw.value())/(rhob.value()*eps[cellI]);
		
	    }
    }
    
    //Sg = 1-Sb;
    
    
    Info << "Sb evaporation: " << " Min(Sb) = " << gMin(Sb) << " Max(Sb) = " << gMax(Sb) << endl;

}
