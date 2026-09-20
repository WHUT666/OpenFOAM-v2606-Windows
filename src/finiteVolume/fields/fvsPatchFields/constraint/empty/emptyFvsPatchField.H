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
    Foam::emptyFvsPatchField

Description
    This boundary condition provides an 'empty' condition for reduced
    dimensions cases.

    The "value" entry is NO_READ, NO_WRITE.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type    empty;
    }
    \endverbatim

SourceFiles
    emptyFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_emptyFvsPatchField_defines_typeName in .C files that
//  define the template statics of emptyFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_emptyFvsPatchField_defines_typeName)
#  define Foam_emptyFvsPatchField_typeName_API
#else
#  define Foam_emptyFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_emptyFvsPatchField_H
#define Foam_emptyFvsPatchField_H

#include "fvsPatchField.H"
#include "emptyFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class emptyFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class emptyFvsPatchField
:
    public fvsPatchField<Type>
{
    typedef emptyFvsPatchField<Type> this_bctype;
    typedef fvsPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_emptyFvsPatchField_typeName_API, emptyFvPatch::typeName_());  // fieldTypes::emptyTypeName_()


    // Constructors

        //- Construct from patch and internal field
        emptyFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        emptyFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        emptyFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        emptyFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        emptyFvsPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const fvPatchFieldMapper&) {}

        //- Reverse map the given fvsPatchField onto this fvsPatchField
        virtual void rmap(const fvsPatchField<Type>&, const labelList&) {}


    // Member Functions

        //- Write without "value" entry!
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment (TBD: could also be ignore assignment)
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "emptyFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
