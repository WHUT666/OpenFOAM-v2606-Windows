/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::cyclicAMIPointPatchField

Description
    Cyclic AMI front and back plane patch field

SourceFiles
    cyclicAMIPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicAMIPointPatchField_defines_typeName in .C files that
//  define the template statics of cyclicAMIPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicAMIPointPatchField_defines_typeName)
#  define Foam_cyclicAMIPointPatchField_typeName_API
#else
#  define Foam_cyclicAMIPointPatchField_typeName_API meshTools_API
#endif

#ifndef Foam_cyclicAMIPointPatchField_H
#define Foam_cyclicAMIPointPatchField_H

#include "coupledPointPatchField.H"
#include "cyclicAMIPointPatch.H"
#include "primitivePatchInterpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class cyclicAMIPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicAMIPointPatchField
:
    public coupledPointPatchField<Type>
{
    typedef cyclicAMIPointPatchField<Type> this_bctype;
    typedef coupledPointPatchField<Type> parent_bctype;
    typedef PrimitivePatchInterpolation<primitivePatch> patchInterp_type;
    //typedef cyclicAMIPointPatch patch_type;

    // Private Data

        //- Local reference cast into the cyclicAMI patch
        const cyclicAMIPointPatch& cyclicAMIPatch_;

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
                        cyclicAMIPatch_.cyclicAMIPatch()
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
                        cyclicAMIPatch_.cyclicAMIPatch().neighbPatch()
                    )
                );
            }
            return *nbrPpiPtr_;
        }


public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicAMIPointPatchField_typeName_API, cyclicAMIPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicAMIPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicAMIPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicAMIPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicAMIPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        cyclicAMIPointPatchField(const this_bctype&) = delete;

        //- Construct with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        // Constraint handling

            //- Return the constraint type this pointPatchField implements
            virtual const word& constraintType() const
            {
                return cyclicAMIPointPatch::typeName;
            }


        // Cyclic AMI coupled interface functions

            //- Does the patch field perform the transformation
            virtual bool doTransform() const
            {
                return
                (
                    is_rotational_vectorspace_v<Type>
                 && !cyclicAMIPatch_.parallel()
                );
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicAMIPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicAMIPatch_.reverseT();
            }


        // Evaluation functions

            //- Return true if coupled. Note that the underlying patch
            //- is not coupled() - the points don't align.
            virtual bool coupled() const;

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
    #include "cyclicAMIPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
