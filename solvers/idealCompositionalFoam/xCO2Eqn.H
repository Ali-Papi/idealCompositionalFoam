{
    fvScalarMatrix xCO2Eqn
        (
            eps*fvm::ddt(Sb,xCO2) + fvm::div(phib,xCO2)
           // ==
            // event source terms
           // - sourceTerm
        );
    xCO2Eqn.solve();
    
    xH2O = 1-xCO2;
    
    Info << "xCO2: " << " Min(xCO2) = " << gMin(xCO2) << " Max(xCO2) = " << gMax(xCO2) << endl;

}
