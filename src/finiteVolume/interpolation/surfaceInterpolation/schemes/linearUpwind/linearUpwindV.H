/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::linearUpwindV

Description
    linearUpwindV interpolation scheme class derived from upwind and returns
    upwind weighting factors but also applies an explicit correction.

SourceFiles
    linearUpwindV.C

\*---------------------------------------------------------------------------*/

//- Define Foam_linearUpwindV_defines_typeName in .C files that
//  define the template statics of linearUpwindV (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_linearUpwindV_defines_typeName)
#  define Foam_linearUpwindV_typeName_API
#else
#  define Foam_linearUpwindV_typeName_API finiteVolume_API
#endif

#ifndef linearUpwindV_H
#define linearUpwindV_H

#include "upwind.H"
#include "gaussGrad.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class linearUpwindV Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class linearUpwindV
:
    public upwind<Type>
{
    // Private Data

        word gradSchemeName_;
        tmp<fv::gradScheme<Type>> gradScheme_;


    // Private Member Functions

        //- No copy construct
        linearUpwindV(const linearUpwindV&) = delete;

        //- No copy assignment
        void operator=(const linearUpwindV&) = delete;


public:

    //- Runtime type information
    TypeNameApi(Foam_linearUpwindV_typeName_API, "linearUpwindV");


    // Constructors

        //- Construct from faceFlux
        linearUpwindV
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux
        )
        :
            upwind<Type>(mesh, faceFlux),
            gradSchemeName_("grad"),
            gradScheme_
            (
                new fv::gaussGrad<Type>(mesh)
            )
        {}

        //- Construct from Istream.
        //  The name of the flux field is read from the Istream and looked-up
        //  from the mesh objectRegistry
        linearUpwindV
        (
            const fvMesh& mesh,
            Istream& schemeData
        )
        :
            upwind<Type>(mesh, schemeData),
            gradSchemeName_(schemeData),
            gradScheme_
            (
                fv::gradScheme<Type>::New
                (
                    mesh,
                    mesh.gradScheme(gradSchemeName_)
                )
            )
        {}

        //- Construct from faceFlux and Istream
        linearUpwindV
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        )
        :
            upwind<Type>(mesh, faceFlux, schemeData),
            gradSchemeName_(schemeData),
            gradScheme_
            (
                fv::gradScheme<Type>::New
                (
                    mesh,
                    mesh.gradScheme(gradSchemeName_)
                )
            )
        {}


    // Member Functions

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return true;
        }

        //- Return the explicit correction to the face-interpolate
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
        correction
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
