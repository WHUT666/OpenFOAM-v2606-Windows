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
    Foam::inletOutletFvPatchField

Group
    grpOutletBoundaryConditions

Description
    This boundary condition provides a generic outflow condition, with
    specified inflow for the case of return flow.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | Flux field name         | no          | phi
        inletValue   | Inlet value for reverse flow | yes    |
        value        | initial field value          | optional |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            inletOutlet;
        phi             phi;
        inletValue      uniform 0;
        value           uniform 0;
    }
    \endverbatim

    The mode of operation is determined by the sign of the flux across the
    patch faces.

Note
    Sign conventions:
    - Positive flux (out of domain): apply zero-gradient condition
    - Negative flux (into domain): apply the "inletValue" fixed-value

See also
    Foam::mixedFvPatchField
    Foam::zeroGradientFvPatchField
    Foam::outletInletFvPatchField

SourceFiles
    inletOutletFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_inletOutletFvPatchField_defines_typeName in .C files that
//  define the template statics of inletOutletFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_inletOutletFvPatchField_defines_typeName)
#  define Foam_inletOutletFvPatchField_typeName_API
#else
#  define Foam_inletOutletFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_inletOutletFvPatchField_H
#define Foam_inletOutletFvPatchField_H

#include "mixedFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class inletOutletFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class inletOutletFvPatchField
:
    public mixedFvPatchField<Type>
{
    typedef inletOutletFvPatchField<Type> this_bctype;
    typedef mixedFvPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- Name of flux field
        word phiName_;


public:

    //- Runtime type information
    TypeNameApi(Foam_inletOutletFvPatchField_typeName_API, "inletOutlet");


    // Constructors

        //- Construct from patch and internal field
        inletOutletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        inletOutletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        inletOutletFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        inletOutletFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        inletOutletFvPatchField(const this_bctype& pfld)
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

        //- True: this patch field is altered by assignment.
        virtual bool assignable() const { return true; }


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
    #include "inletOutletFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
