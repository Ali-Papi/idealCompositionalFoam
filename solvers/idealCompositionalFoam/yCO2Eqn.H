{
   // surfaceScalarField PHf = fvc::interpolate(PH,"PHf");
    fvScalarMatrix yCO2Eqn
        (
            eps*(fvm::ddt(yCO2) - fvm::ddt(Sb,yCO2)) + fvm::div(phia,yCO2)
            ==
            // event source terms
            - sourceTerm
        );
    yCO2Eqn.solve();
    
     yH2O= 1-yCO2;

    Info << "yH2O: " << " Min(yH2O) = " << gMin(yH2O) << " Max(yH2O) = " << gMax(yH2O) << endl;

}
