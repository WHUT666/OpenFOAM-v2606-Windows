/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

Class
    Foam::fa::gaussConvectionScheme

Description
    Basic second-order convection using face-gradients and Gauss' theorem.

SourceFiles
    gaussFaConvectionScheme.C
    gaussFaConvectionSchemes.C

\*---------------------------------------------------------------------------*/

//- Define Foam_gaussConvectionScheme_defines_typeName in .C files that
//  define the template statics of gaussConvectionScheme (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_gaussConvectionScheme_defines_typeName)
#  define Foam_gaussConvectionScheme_typeName_API
#else
#  define Foam_gaussConvectionScheme_typeName_API finiteArea_API
#endif

#ifndef gaussFaConvectionScheme_H
#define gaussFaConvectionScheme_H

#include "faConvectionScheme.H"
#include "edgeInterpolationScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fa
{

/*---------------------------------------------------------------------------*\
                    Class gaussConvectionScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class gaussConvectionScheme
:
    public fa::convectionScheme<Type>
{
    // Private Data

        //- Edge-interpolation scheme
        tmp<edgeInterpolationScheme<Type>> tinterpScheme_;


public:

    //- Runtime type information
    TypeNameApi(Foam_gaussConvectionScheme_typeName_API, "Gauss");


    // Generated Methods

        //- No copy construct
        gaussConvectionScheme(const gaussConvectionScheme&) = delete;

        //- No copy assignment
        void operator=(const gaussConvectionScheme&) = delete;


    // Constructors

        //- Construct from flux and interpolation scheme
        gaussConvectionScheme
        (
            const faMesh& mesh,
            const edgeScalarField& faceFlux,
            const tmp<edgeInterpolationScheme<Type>>& scheme
        )
        :
            convectionScheme<Type>(mesh, faceFlux),
            tinterpScheme_(scheme)
        {}

        //- Construct from flux and Istream
        gaussConvectionScheme
        (
            const faMesh& mesh,
            const edgeScalarField& faceFlux,
            Istream& is
        )
        :
            convectionScheme<Type>(mesh, faceFlux),
            tinterpScheme_
            (
                edgeInterpolationScheme<Type>::New(mesh, faceFlux, is)
            )
        {}


    // Member Functions

        tmp<GeometricField<Type, faePatchField, edgeMesh>> flux
        (
            const edgeScalarField&,
            const GeometricField<Type, faPatchField, areaMesh>&
        ) const;

        tmp<faMatrix<Type>> famDiv
        (
            const edgeScalarField&,
            const GeometricField<Type, faPatchField, areaMesh>&
        ) const;

        tmp<GeometricField<Type, faPatchField, areaMesh>> facDiv
        (
            const edgeScalarField&,
            const GeometricField<Type, faPatchField, areaMesh>&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fa

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "gaussFaConvectionScheme.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
