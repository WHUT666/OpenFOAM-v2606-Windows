/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018-2019 OpenCFD Ltd.
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
    Foam::jumpCyclicFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition provides a base class for coupled-cyclic
    conditions with a specified 'jump' (or offset) between the values

    The "value" entry is NO_READ.

See also
    Foam::cyclicFvPatchField

SourceFiles
    jumpCyclicFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_jumpCyclicFvPatchField_defines_typeName in .C files that
//  define the template statics of jumpCyclicFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_jumpCyclicFvPatchField_defines_typeName)
#  define Foam_jumpCyclicFvPatchField_typeName_API
#else
#  define Foam_jumpCyclicFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_jumpCyclicFvPatchField_H
#define Foam_jumpCyclicFvPatchField_H

#include "cyclicFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class jumpCyclicFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class jumpCyclicFvPatchField
:
    public cyclicFvPatchField<Type>
{
    typedef jumpCyclicFvPatchField<Type> this_bctype;
    typedef cyclicFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_jumpCyclicFvPatchField_typeName_API, "jumpCyclic");


    // Constructors

        //- Construct from patch and internal field
        jumpCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        jumpCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&,
            const bool needValue = true  //!< evaluate
        );

        //- Construct by mapping onto a new patch
        jumpCyclicFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        jumpCyclicFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        jumpCyclicFvPatchField(const this_bctype& pfld)
        #ifdef Foam_fvPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif


    // Member Functions

        // Access

            //- Return the interface type
            virtual const word& interfaceFieldType() const
            {
                return cyclicFvPatchField<Type>::type();
            }

            //- Return the "jump" across the patch as a "half" field
            virtual tmp<Field<Type>> jump() const = 0;


        // Evaluation functions

            //- Return neighbour coupled values
            virtual tmp<Field<Type>> patchNeighbourField() const;

            //- Retrieve neighbour coupled values
            virtual void patchNeighbourField(UList<Type>& pnf) const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                solveScalarField& result,
                const bool add,
                const lduAddressing& lduAddr,
                const label patchId,
                const solveScalarField& psiInternal,
                const scalarField& coeffs,
                const direction cmpt,
                const Pstream::commsTypes commsType
            ) const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                Field<Type>&,
                const bool add,
                const lduAddressing& lduAddr,
                const label patchId,
                const Field<Type>&,
                const scalarField&,
                const Pstream::commsTypes commsType
            ) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};

// Specializations

//- Update result field based on interface functionality
template<>
void jumpCyclicFvPatchField<scalar>::updateInterfaceMatrix
(
    solveScalarField& result,
    const bool add,
    const lduAddressing& lduAddr,
    const label patchId,
    const solveScalarField& psiInternal,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes commsType
) const;


template<>
void jumpCyclicFvPatchField<vector>::updateInterfaceMatrix
(
    solveScalarField& result,
    const bool add,
    const lduAddressing& lduAddr,
    const label patchId,
    const solveScalarField& psiInternal,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes commsType
) const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "jumpCyclicFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
