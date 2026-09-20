/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2026 OpenCFD Ltd.
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
    Foam::genericFvsPatchField

Description
    This boundary condition provides a generic version of the \c calculated
    condition, useful as a fallback for handling unknown patch types.  Not
    generally applicable as a user-specified condition.

See also
    Foam::calculatedFvsPatchField

SourceFiles
    genericFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_genericFvsPatchField_defines_typeName in .C files that
//  define the template statics of genericFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_genericFvsPatchField_defines_typeName)
#  define Foam_genericFvsPatchField_typeName_API
#else
#  define Foam_genericFvsPatchField_typeName_API genericPatchFields_API
#endif

#ifndef Foam_genericFvsPatchField_H
#define Foam_genericFvsPatchField_H

#include "calculatedFvsPatchField.H"
#include "genericPatchFieldBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class genericFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class genericFvsPatchField
:
    public genericPatchFieldBase,
    public calculatedFvsPatchField<Type>
{
    typedef genericFvsPatchField<Type> this_bctype;
    typedef calculatedFvsPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_genericFvsPatchField_typeName_API, "generic");


    // Constructors

        //- Construct from patch and internal field
        genericFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary
        genericFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        genericFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Copy construct setting internal field reference
        genericFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        genericFvsPatchField(const this_bctype& pfld)
        :
            this_bctype(pfld, pfld.internalField())
        {}

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Write
        virtual void write(Ostream&) const;


    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const fvPatchFieldMapper&);

        //- Reverse map the given fvsPatchField onto this fvsPatchField
        virtual void rmap(const fvsPatchField<Type>&, const labelList&);


    // Evaluation Functions

        //- Fatal
        virtual tmp<Field<Type>>
        valueInternalCoeffs(const tmp<scalarField>&) const;

        //- Fatal
        virtual tmp<Field<Type>>
        valueBoundaryCoeffs(const tmp<scalarField>&) const;

        //- Fatal
        tmp<Field<Type>> gradientInternalCoeffs() const;

        //- Fatal
        tmp<Field<Type>> gradientBoundaryCoeffs() const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "genericFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
