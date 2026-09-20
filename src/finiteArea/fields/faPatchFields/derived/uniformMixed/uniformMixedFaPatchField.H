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
    Foam::uniformMixedFaPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition provides 'mixed' type boundary condition
    that mix a \em uniform fixed value and a \em uniform patch-normal
    gradient condition. The term "uniform" is a legacy name since the
    prescribed values were previously spatially uniform across that patch.

Usage
    \table
        Property     | Description                      | Required | Default
        uniformValue | uniform value                    | partly | 0
        uniformGradient | uniform gradient              | partly | 0
        uniformValueFraction | uniform valueFraction    | partly | depends
        value        | initial field value              | optional |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformMixed;
        uniformValue    constant 0.2;
        uniformGradient constant 0.2;
        uniformValueFraction
        {
            type  sine;
            ...
        }
    }
    \endverbatim

Note
    This boundary condition allows \em lazier definitions so that either
    or both: \c uniformValue and \c uniformGradient must be defined.
    If only of these entries is defined, the value fraction is automatically
    treated appropriately (ie, 0 with \c uniformGradient and 1 with
    uniformValue).
    If both \c uniformValue and \c uniformGradient are defined,
    the \c uniformValueFraction must also be defined.

    The \c value entry (optional) is used for the initial values.
    Otherwise the function(s) are used for the evaluation.
    In some cases (eg, coded or expression entries with references to other
    fields) this can be problematic and the \c value entry will be needed.

See also
    Foam::Function1Types
    Foam::mixedFaPatchField

SourceFiles
    uniformMixedFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_uniformMixedFaPatchField_defines_typeName in .C files that
//  define the template statics of uniformMixedFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_uniformMixedFaPatchField_defines_typeName)
#  define Foam_uniformMixedFaPatchField_typeName_API
#else
#  define Foam_uniformMixedFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_uniformMixedFaPatchField_H
#define Foam_uniformMixedFaPatchField_H

#include "mixedFaPatchField.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class uniformMixedFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformMixedFaPatchField
:
    public mixedFaPatchField<Type>
{
    typedef uniformMixedFaPatchField<Type> this_bctype;
    typedef mixedFaPatchField<Type> parent_bctype;

    // Private Data

        //- Function providing the value
        autoPtr<Function1<Type>> refValueFunc_;

        //- Function providing the gradient
        autoPtr<Function1<Type>> refGradFunc_;

        //- Function providing the value-fraction
        autoPtr<Function1<scalar>> valueFractionFunc_;


public:

    //- Runtime type information
    TypeNameApi(Foam_uniformMixedFaPatchField_typeName_API, "uniformMixed");


    // Constructors

        //- Construct from patch and internal field
        uniformMixedFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch and internal field and patch field
        uniformMixedFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const Field<Type>& fld
        );

        //- Construct from patch, internal field and dictionary
        uniformMixedFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformMixedFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformMixedFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        uniformMixedFaPatchField(const this_bctype&) = delete;

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

        //- Write includes "value" entry (for visualisation / restart)
        virtual void write(Ostream& os) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "uniformMixedFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
