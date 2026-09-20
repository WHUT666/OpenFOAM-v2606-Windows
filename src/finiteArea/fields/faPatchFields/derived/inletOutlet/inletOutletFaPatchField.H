/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2019 OpenCFD Ltd.
    Copyright (C) 2026 Keysight Technologies
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
    Foam::inletOutletFaPatchField

Group
    grpOutletBoundaryConditions

Description
    This boundary condition provides a generic outflow condition, with
    specified inflow for the case of return flow.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | Flux field name         | no          | phis
        inletValue   | Inlet value for reverse flow | yes    |
        value        | initial field value          | optional |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            inletOutlet;
        phi             phis;
        inletValue      uniform 0;
        value           uniform 0;
    }
    \endverbatim

    The mode of operation is determined by the sign of the flux across the
    patch edges.

Note
    Sign conventions:
    - Positive flux (out of domain): apply zero-gradient condition
    - Negative flux (into domain): apply the "inletValue" fixed-value

See also
    Foam::mixedFaPatchField
    Foam::zeroGradientFaPatchField
    Foam::outletInletFaPatchField

SourceFiles
    inletOutletFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_inletOutletFaPatchField_defines_typeName in .C files that
//  define the template statics of inletOutletFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_inletOutletFaPatchField_defines_typeName)
#  define Foam_inletOutletFaPatchField_typeName_API
#else
#  define Foam_inletOutletFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_inletOutletFaPatchField_H
#define Foam_inletOutletFaPatchField_H

#include "mixedFaPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class inletOutletFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class inletOutletFaPatchField
:
    public mixedFaPatchField<Type>
{
    typedef inletOutletFaPatchField<Type> this_bctype;
    typedef mixedFaPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- Name of flux field
        word phiName_;


public:

    //- Runtime type information
    TypeNameApi(Foam_inletOutletFaPatchField_typeName_API, "inletOutlet");


    // Constructors

        //- Construct from patch and internal field
        inletOutletFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and dictionary
        inletOutletFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        inletOutletFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        inletOutletFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        inletOutletFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~inletOutletFaPatchField() = default;


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
    #include "inletOutletFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
