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
    Foam::outletInletFvPatchField

Group
    grpInletBoundaryConditions

Description
    This boundary condition provides a generic inflow condition, with
    specified outflow for the case of reverse flow.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | Flux field name         | no          | phi
        outletValue  | Outlet value for reverse flow | yes   |
        value        | initial field value     | optional |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            outletInlet;
        phi             phi;
        outletValue     uniform 0;
        value           uniform 0;
    }
    \endverbatim

    The mode of operation is determined by the sign of the flux across the
    patch faces.

Note
    Sign conventions:
    - Positive flux (out of domain): apply the "outletValue" fixed-value
    - Negative flux (into domain): apply zero-gradient condition

See also
    Foam::mixedFvPatchField
    Foam::zeroGradientFvPatchField
    Foam::inletOutletFvPatchField

SourceFiles
    outletInletFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_outletInletFvPatchField_defines_typeName in .C files that
//  define the template statics of outletInletFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_outletInletFvPatchField_defines_typeName)
#  define Foam_outletInletFvPatchField_typeName_API
#else
#  define Foam_outletInletFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_outletInletFvPatchField_H
#define Foam_outletInletFvPatchField_H

#include "mixedFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class outletInletFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class outletInletFvPatchField
:
    public mixedFvPatchField<Type>
{
    typedef outletInletFvPatchField<Type> this_bctype;
    typedef mixedFvPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- Name of flux field (default: phi)
        word phiName_;


public:

    //- Runtime type information
    TypeNameApi(Foam_outletInletFvPatchField_typeName_API, "outletInlet");


    // Constructors

        //- Construct from patch and internal field
        outletInletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        outletInletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        outletInletFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        outletInletFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        outletInletFvPatchField(const this_bctype& pfld)
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

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

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
    #include "outletInletFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
