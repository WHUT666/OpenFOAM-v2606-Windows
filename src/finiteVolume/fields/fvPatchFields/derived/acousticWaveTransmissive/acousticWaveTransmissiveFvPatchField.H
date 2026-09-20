/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::acousticWaveTransmissiveFvPatchField

Group
    grpOutletBoundaryConditions

Description
    This boundary condition provides a wave transmissive outflow condition,
    based on solving DDt(W, field) = 0 at the boundary \c W is the wave velocity
    and \c field is the field to which this boundary condition is applied.
    The wave speed is input in the BC.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        // Mandatory entries (unmodifiable)
        type                acousticWaveTransmissive;
        advectiveSpeed      50.0;

        // Mandatory/Optional (inherited) entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                          | Type  | Reqd | Dflt
      type     | Type name: acousticWaveTransmissive  | word  | yes  | -
      advectiveSpeed | Advective speed value          | scalar | yes | -
    \endtable

    The inherited entries are elaborated in:
      - \link advectiveFvPatchField.H \endlink

See also
  - Foam::advectiveFvPatchFields

SourceFiles
    acousticWaveTransmissiveFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_acousticWaveTransmissiveFvPatchField_defines_typeName in .C files that
//  define the template statics of acousticWaveTransmissiveFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_acousticWaveTransmissiveFvPatchField_defines_typeName)
#  define Foam_acousticWaveTransmissiveFvPatchField_typeName_API
#else
#  define Foam_acousticWaveTransmissiveFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_acousticWaveTransmissiveFvPatchField_H
#define Foam_acousticWaveTransmissiveFvPatchField_H

#include "advectiveFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class acousticWaveTransmissiveFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class acousticWaveTransmissiveFvPatchField
:
    public advectiveFvPatchField<Type>
{
    typedef acousticWaveTransmissiveFvPatchField<Type> this_bctype;
    typedef advectiveFvPatchField<Type> parent_bctype;

    // Private Data

        //- Advection speed value
        scalar advectiveU_;


public:

    //- Runtime type information
    TypeNameApi(Foam_acousticWaveTransmissiveFvPatchField_typeName_API, "acousticWaveTransmissive");


    // Constructors

        //- Construct from patch and internal field
        acousticWaveTransmissiveFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        acousticWaveTransmissiveFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        acousticWaveTransmissiveFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        acousticWaveTransmissiveFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        acousticWaveTransmissiveFvPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        // Evaluation

            //- Calculate and return the advection speed at the boundary
            virtual tmp<scalarField> advectionSpeed() const;


        // IO

            //- Write
            virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "acousticWaveTransmissiveFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
