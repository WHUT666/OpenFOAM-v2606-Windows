/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
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
    Foam::uniformInletOutletFvPatchField

Group
    grpOutletBoundaryConditions

Description
    Variant of inletOutlet boundary condition with uniform inletValue.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | flux field name         | no          | phi
        uniformInletValue   | inlet value for reverse flow | yes    |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type                uniformInletOutlet;
        phi                 phi;
        uniformInletValue   0;
        value               uniform 0;
    }
    \endverbatim

    The mode of operation is determined by the sign of the flux across the
    patch faces.

Note
    Sign conventions:
    - positive flux (out of domain): apply zero-gradient condition
    - negative flux (into of domain): apply the user-specified fixed value

See also
    Foam::inletOutletFvPatchField
    Foam::Function1Types

SourceFiles
    uniformInletOutletFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_uniformInletOutletFvPatchField_defines_typeName in .C files that
//  define the template statics of uniformInletOutletFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_uniformInletOutletFvPatchField_defines_typeName)
#  define Foam_uniformInletOutletFvPatchField_typeName_API
#else
#  define Foam_uniformInletOutletFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_uniformInletOutletFvPatchField_H
#define Foam_uniformInletOutletFvPatchField_H

#include "mixedFvPatchField.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class uniformInletOutletFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformInletOutletFvPatchField
:
    public mixedFvPatchField<Type>
{
    typedef uniformInletOutletFvPatchField<Type> this_bctype;
    typedef mixedFvPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- Name of flux field
        word phiName_;

        //- Value
        autoPtr<Function1<Type>> uniformInletValue_;


public:

    //- Runtime type information
    TypeNameApi(Foam_uniformInletOutletFvPatchField_typeName_API, "uniformInletOutlet");


    // Constructors

        //- Construct from patch and internal field
        uniformInletOutletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformInletOutletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformInletOutletFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformInletOutletFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        uniformInletOutletFvPatchField(const this_bctype& pfld)
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


    // Member functions

        //- True: this patch field is altered by assignment
        virtual bool assignable() const { return true; }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<Type>&,
                const labelList&
            );


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        virtual void operator=(const fvPatchField<Type>& pvf);

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "uniformInletOutletFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
