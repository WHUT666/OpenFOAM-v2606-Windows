/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2023 OpenCFD Ltd.
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
    Foam::uniformFixedValueFaPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition provides a uniform fixed value condition.

Usage
    \table
        Property     | Description                  | Required | Default
        uniformValue | uniform value                | yes |
        value        | initial field value          | optional |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformFixedValue;
        uniformValue    constant 0.2;
    }
    \endverbatim

Note
    The uniformValue entry is a Function1 type,
    able to describe time varying functions.
    The example above gives the usage for supplying a constant value.

    The \c value entry (optional) is used for the initial values.
    Otherwise the \c uniformValue is evaluated.
    In some cases (eg, coded or expression entries with references to other
    fields) this can be problematic and the \c value entry will be needed.

See also
    Foam::Function1Types
    Foam::fixedValueFaPatchField

SourceFiles
    uniformFixedValueFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_uniformFixedValueFaPatchField_defines_typeName in .C files that
//  define the template statics of uniformFixedValueFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_uniformFixedValueFaPatchField_defines_typeName)
#  define Foam_uniformFixedValueFaPatchField_typeName_API
#else
#  define Foam_uniformFixedValueFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_uniformFixedValueFaPatchField_H
#define Foam_uniformFixedValueFaPatchField_H

#include "fixedValueFaPatchField.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class uniformFixedValueFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformFixedValueFaPatchField
:
    public fixedValueFaPatchField<Type>
{
    typedef uniformFixedValueFaPatchField<Type> this_bctype;
    typedef fixedValueFaPatchField<Type> parent_bctype;

    // Private Data

        //- Function providing the value
        autoPtr<Function1<Type>> refValueFunc_;


public:

    //- Runtime type information
    TypeNameApi(Foam_uniformFixedValueFaPatchField_typeName_API, "uniformFixedValue");


    // Constructors

        //- Construct from patch and internal field
        uniformFixedValueFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch and internal field and patch field
        uniformFixedValueFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const Field<Type>& fld
        );

        //- Construct from patch, internal field and dictionary
        uniformFixedValueFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformFixedValueFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformFixedValueFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        uniformFixedValueFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write includes "value" entry
        virtual void write(Ostream& os) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "uniformFixedValueFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
