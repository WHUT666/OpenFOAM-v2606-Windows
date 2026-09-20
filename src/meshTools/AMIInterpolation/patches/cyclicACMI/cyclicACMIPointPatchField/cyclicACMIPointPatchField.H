/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2017 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::cyclicACMIPointPatchField

Description
    Cyclic ACMI front and back plane patch field

SourceFiles
    cyclicACMIPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicACMIPointPatchField_defines_typeName in .C files that
//  define the template statics of cyclicACMIPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicACMIPointPatchField_defines_typeName)
#  define Foam_cyclicACMIPointPatchField_typeName_API
#else
#  define Foam_cyclicACMIPointPatchField_typeName_API meshTools_API
#endif

#ifndef Foam_cyclicACMIPointPatchField_H
#define Foam_cyclicACMIPointPatchField_H

#include "coupledPointPatchField.H"
#include "cyclicACMIPointPatch.H"
#include "primitivePatchInterpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class cyclicACMIPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicACMIPointPatchField
:
    public coupledPointPatchField<Type>
{
    typedef cyclicACMIPointPatchField<Type> this_bctype;
    typedef coupledPointPatchField<Type> parent_bctype;
    typedef PrimitivePatchInterpolation<primitivePatch> patchInterp_type;
    //typedef cyclicACMIPointPatch patch_type;

    // Private Data

        //- Local reference cast into the cyclicACMI patch
        const cyclicACMIPointPatch& cyclicACMIPatch_;

        //- Owner side patch interpolation pointer
        mutable autoPtr<patchInterp_type> ppiPtr_;

        //- Neighbour side patch interpolation pointer
        mutable autoPtr<patchInterp_type> nbrPpiPtr_;


    // Private Member Functions

        //- Owner side patch interpolation
        const PrimitivePatchInterpolation<primitivePatch>& ppi() const
        {
            if (!ppiPtr_)
            {
                ppiPtr_.reset
                (
                    new patchInterp_type
                    (
                        cyclicACMIPatch_.cyclicACMIPatch()
                    )
                );
            }
            return *ppiPtr_;
        }

        //- Neighbour side patch interpolation
        const PrimitivePatchInterpolation<primitivePatch>& nbrPpi() const
        {
            if (!nbrPpiPtr_)
            {
                nbrPpiPtr_.reset
                (
                    new patchInterp_type
                    (
                        cyclicACMIPatch_.cyclicACMIPatch().neighbPatch()
                    )
                );
            }
            return *nbrPpiPtr_;
        }


public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicACMIPointPatchField_typeName_API, cyclicACMIPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicACMIPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicACMIPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicACMIPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicACMIPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        cyclicACMIPointPatchField(const this_bctype&) = delete;

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
                return cyclicACMIPointPatch::typeName;
            }


        // Cyclic AMI coupled interface functions

            //- Does the patch field perform the transformation
            virtual bool doTransform() const
            {
                return
                (
                    is_rotational_vectorspace_v<Type>
                 && !cyclicACMIPatch_.parallel()
                );
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicACMIPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicACMIPatch_.reverseT();
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
    #include "cyclicACMIPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
