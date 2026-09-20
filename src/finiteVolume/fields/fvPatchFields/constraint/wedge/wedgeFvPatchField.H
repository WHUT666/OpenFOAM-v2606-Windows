/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::wedgeFvPatchField

Group
    grpConstraintBoundaryConditions

Description
    This boundary condition is similar to the cyclic condition, except that
    it is applied to 2-D geometries.

    The "value" entry is NO_READ, NO_WRITE.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            wedge;
    }
    \endverbatim

See also
    Foam::cyclicFvPatchField

SourceFiles
    wedgeFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_wedgeFvPatchField_defines_typeName in .C files that
//  define the template statics of wedgeFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_wedgeFvPatchField_defines_typeName)
#  define Foam_wedgeFvPatchField_typeName_API
#else
#  define Foam_wedgeFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_wedgeFvPatchField_H
#define Foam_wedgeFvPatchField_H

#include "transformFvPatchField.H"
#include "wedgeFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class wedgeFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class wedgeFvPatchField
:
    public transformFvPatchField<Type>
{
    typedef wedgeFvPatchField<Type> this_bctype;
    typedef transformFvPatchField<Type> parent_bctype;


public:

    //- Runtime type information
    TypeNameApi(Foam_wedgeFvPatchField_typeName_API, wedgeFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        wedgeFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        wedgeFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        wedgeFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        wedgeFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        wedgeFvPatchField(const this_bctype& pfld)
        #ifdef Foam_fvPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Return gradient at boundary
        virtual tmp<Field<Type>> snGrad() const;

        //- Retrieve patch-normal gradient [contiguous storage]
        virtual void snGrad(UList<Type>& result) const;

        //- Evaluate the patch field
        virtual void evaluate
        (
            const Pstream::commsTypes commsType=Pstream::commsTypes::buffered
        );

        //- Return face-gradient transform diagonal
        virtual tmp<Field<Type>> snGradTransformDiag() const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "wedgeFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
