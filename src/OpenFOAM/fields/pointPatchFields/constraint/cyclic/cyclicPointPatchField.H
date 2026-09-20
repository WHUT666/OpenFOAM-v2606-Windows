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
    Foam::cyclicPointPatchField

Description
    Cyclic front and back plane patch field

SourceFiles
    cyclicPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicPointPatchField_defines_typeName in .C files that
//  define the template statics of cyclicPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicPointPatchField_defines_typeName)
#  define Foam_cyclicPointPatchField_typeName_API
#else
#  define Foam_cyclicPointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_cyclicPointPatchField_H
#define Foam_cyclicPointPatchField_H

#include "coupledPointPatchField.H"
#include "cyclicPointPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class cyclicPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicPointPatchField
:
    public coupledPointPatchField<Type>
{
    typedef cyclicPointPatchField<Type> this_bctype;
    typedef coupledPointPatchField<Type> parent_bctype;
    //typedef cyclicPointPatch patch_type;

    // Private Data

        //- Local reference cast into the cyclic patch
        const cyclicPointPatch& cyclicPatch_;


public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicPointPatchField_typeName_API, cyclicPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        cyclicPointPatchField(const this_bctype& pfld)
        #ifdef Foam_pointPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member functions

        // Constraint handling

            //- Return the constraint type this pointPatchField implements
            virtual const word& constraintType() const
            {
                return cyclicPointPatch::typeName;
            }


        // Cyclic coupled interface functions

            //- Does the patch field perform the transformation
            virtual bool doTransform() const
            {
                return
                (
                    is_rotational_vectorspace_v<Type>
                 && !cyclicPatch_.parallel()
                );
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicPatch_.reverseT();
            }


        // Evaluation functions

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::buffered
            )
            {}

            //- Complete swap of patch point values and add to local values
            virtual void swapAddSeparated
            (
                const Pstream::commsTypes commsType,
                Field<Type>&
            ) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cyclicPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
