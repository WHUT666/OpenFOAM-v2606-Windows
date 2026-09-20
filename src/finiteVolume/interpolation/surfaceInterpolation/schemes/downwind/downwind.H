/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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
    Foam::downwind

Group
    grpFvSurfaceInterpolationSchemes

Description
    Downwind differencing scheme class.

SourceFiles
    downwind.C

\*---------------------------------------------------------------------------*/

//- Define Foam_downwind_defines_typeName in .C files that
//  define the template statics of downwind (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_downwind_defines_typeName)
#  define Foam_downwind_typeName_API
#else
#  define Foam_downwind_typeName_API finiteVolume_API
#endif

#ifndef downwind_H
#define downwind_H

#include "surfaceInterpolationScheme.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class downwind Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class downwind
:
    public surfaceInterpolationScheme<Type>
{
    // Private data

        const surfaceScalarField& faceFlux_;


    // Private Member Functions

        //- No copy assignment
        void operator=(const downwind&) = delete;


public:

    //- Runtime type information
    TypeNameApi(Foam_downwind_typeName_API, "downwind");


    // Constructors

        //- Construct from faceFlux
        downwind
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux
        )
        :
            surfaceInterpolationScheme<Type>(mesh),
            faceFlux_(faceFlux)
        {}

        //- Construct from Istream.
        //  The name of the flux field is read from the Istream and looked-up
        //  from the mesh objectRegistry
        downwind
        (
            const fvMesh& mesh,
            Istream& is
        )
        :
            surfaceInterpolationScheme<Type>(mesh),
            faceFlux_
            (
                mesh.lookupObject<surfaceScalarField>
                (
                    word(is)
                )
            )
        {}

        //- Construct from faceFlux and Istream
        downwind
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream&
        )
        :
            surfaceInterpolationScheme<Type>(mesh),
            faceFlux_(faceFlux)
        {}


    // Member Functions

        //- Return the interpolation weighting factors
        virtual tmp<surfaceScalarField> weights
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const
        {
            return neg(faceFlux_);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
