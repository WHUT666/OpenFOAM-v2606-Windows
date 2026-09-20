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
    Foam::processorCyclicFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition enables processor communication across cyclic
    patches.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            processor;
    }
    \endverbatim

See also
    Foam::processorFvPatchField

SourceFiles
    processorCyclicFvPatchField.txx
    processorCyclicFvPatchFields.H
    processorCyclicFvPatchFields.cxx
    processorCyclicFvPatchFieldsFwd.H

\*---------------------------------------------------------------------------*/

//- Define Foam_processorCyclicFvPatchField_defines_typeName in .C files that
//  define the template statics of processorCyclicFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_processorCyclicFvPatchField_defines_typeName)
#  define Foam_processorCyclicFvPatchField_typeName_API
#else
#  define Foam_processorCyclicFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_processorCyclicFvPatchField_H
#define Foam_processorCyclicFvPatchField_H

#include "processorCyclicFvPatch.H"
#include "processorFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class processorCyclicFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorCyclicFvPatchField
:
    public processorFvPatchField<Type>
{
    typedef processorCyclicFvPatchField<Type> this_bctype;
    typedef processorFvPatchField<Type> parent_bctype;

    // Private Data

        //- Local reference cast into the processor patch
        const processorCyclicFvPatch& procPatch_;

public:

    //- Runtime type information
    TypeNameApi(Foam_processorCyclicFvPatchField_typeName_API, processorCyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch and internal field and patch field
        processorCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        processorCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        processorCyclicFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        processorCyclicFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        processorCyclicFvPatchField(const this_bctype& pfld)
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


    //- Destructor
    virtual ~processorCyclicFvPatchField() = default;


    // Member Functions

        //- Does the patch field perform the transformation
        virtual bool doTransform() const
        {
            return
            (
                is_rotational_vectorspace_v<Type>
             && !procPatch_.parallel()
            );
        }

        //- Return face transformation tensor
        virtual const tensorField& forwardT() const
        {
            return procPatch_.forwardT();
        }


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "processorCyclicFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
