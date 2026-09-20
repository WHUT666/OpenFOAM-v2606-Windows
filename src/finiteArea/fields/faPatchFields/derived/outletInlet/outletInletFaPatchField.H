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
    Foam::outletInletFaPatchField

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
    patch edges.

Note
    Sign conventions:
    - Positive flux (out of domain): apply the "outletValue" fixed-value
    - Negative flux (into domain): apply zero-gradient condition

See also
    Foam::mixedFaPatchField
    Foam::zeroGradientFaPatchField
    Foam::inletOutletFaPatchField

SourceFiles
    outletInletFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_outletInletFaPatchField_defines_typeName in .C files that
//  define the template statics of outletInletFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_outletInletFaPatchField_defines_typeName)
#  define Foam_outletInletFaPatchField_typeName_API
#else
#  define Foam_outletInletFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_outletInletFaPatchField_H
#define Foam_outletInletFaPatchField_H

#include "mixedFaPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class outletInletFaPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class outletInletFaPatchField
:
    public mixedFaPatchField<Type>
{
    typedef outletInletFaPatchField<Type> this_bctype;
    typedef mixedFaPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- Name of flux field (default: phi)
        word phiName_;


public:

    //- Runtime type information
    TypeNameApi(Foam_outletInletFaPatchField_typeName_API, "outletInlet");


    // Constructors

        //- Construct from patch and internal field
        outletInletFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and dictionary
        outletInletFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        outletInletFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        outletInletFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        outletInletFaPatchField(const this_bctype&) = delete;

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
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "outletInletFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
