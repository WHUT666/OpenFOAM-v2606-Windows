/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
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
    Foam::extrapolatedCalculatedFaPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition applies a zero-gradient condition from the patch
    internal field onto the patch faces when \c evaluated but may also be
    assigned.  \c snGrad returns the patch gradient evaluated from the current
    internal and patch field values rather than returning zero.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            extrapolatedCalculated;
    }
    \endverbatim

SourceFiles
    extrapolatedCalculatedFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_extrapolatedCalculatedFaPatchField_defines_typeName in .C files that
//  define the template statics of extrapolatedCalculatedFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_extrapolatedCalculatedFaPatchField_defines_typeName)
#  define Foam_extrapolatedCalculatedFaPatchField_typeName_API
#else
#  define Foam_extrapolatedCalculatedFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_extrapolatedCalculatedFaPatchField_H
#define Foam_extrapolatedCalculatedFaPatchField_H

#include "calculatedFaPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class extrapolatedCalculatedFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class extrapolatedCalculatedFaPatchField
:
    public calculatedFaPatchField<Type>
{
    typedef extrapolatedCalculatedFaPatchField<Type> this_bctype;
    typedef calculatedFaPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_extrapolatedCalculatedFaPatchField_typeName_API, "extrapolatedCalculated");
    // fieldTypes::extrapolatedCalculatedTypeName_()


    // Constructors

        //- Construct from patch and internal field
        extrapolatedCalculatedFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and dictionary
        extrapolatedCalculatedFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        extrapolatedCalculatedFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        extrapolatedCalculatedFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        extrapolatedCalculatedFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Evaluate the patch field
        virtual void evaluate
        (
            const Pstream::commsTypes commsType=Pstream::commsTypes::buffered
        );


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "extrapolatedCalculatedFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
