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
    Foam::interpolationPointMVC

Description
    Given cell centre values interpolates to vertices and uses these to
    do a Mean Value Coordinates interpolation.

\*---------------------------------------------------------------------------*/

//- Define Foam_interpolationPointMVC_defines_typeName in .C files that
//  define the template statics of interpolationPointMVC (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_interpolationPointMVC_defines_typeName)
#  define Foam_interpolationPointMVC_typeName_API
#else
#  define Foam_interpolationPointMVC_typeName_API finiteVolume_API
#endif

#ifndef Foam_interpolationPointMVC_H
#define Foam_interpolationPointMVC_H

#include "interpolation.H"
#include "pointMVCWeight.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class interpolationPointMVC Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolationPointMVC
:
    public interpolation<Type>
{
protected:

    // Protected Data

        //- Interpolated volfield
        const GeometricField<Type, pointPatchField, pointMesh> psip_;


public:

    //- Runtime type information
    TypeNameApi(Foam_interpolationPointMVC_typeName_API, "pointMVC");


    // Constructors

        //- Construct from components
        explicit interpolationPointMVC
        (
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    // Member Functions

        //- Inherit interpolate from interpolation
        using interpolation<Type>::interpolate;

        //- Interpolate field for the given cellPointWeight
        inline Type interpolate(const pointMVCWeight& cpw) const;

        //- Interpolate field to the given point in the given cell
        inline Type interpolate
        (
            const vector& position,
            const label celli,
            const label facei = -1
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "interpolationPointMVCI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "interpolationPointMVC.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
