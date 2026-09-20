/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::SRFVelocityFvPatchVectorField

Group
    grpInletBoundaryConditions grpWallBoundaryConditions

Description
    Velocity condition to be used in conjunction with the single
    rotating frame (SRF) model (see: SRFModel class)

    Given the free stream velocity in the absolute frame, the condition
    applies the appropriate rotation transformation in time and space to
    determine the local velocity.

    The optional \c relative flag switches the behaviour of the patch
    such that:

        - relative = yes: inlet velocity applied 'as is':

        \f[
            U_p = U_{in}
        \f]

        - relative = no : SRF velocity is subtracted from the inlet velocity:

        \f[
            U_p = U_{in} - U_{p,srf}
        \f]

    where
    \vartable
        U_p     | patch velocity [m/s]
        U_{in}  | user-specified inlet velocity
        U_{p,srf} | SRF velocity
    \endvartable

Usage
    \table
        Property     | Description             | Required    | Default value
        inletValue   | inlet velocity          | yes         |
        relative     | inletValue relative motion to the SRF? | yes     |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            SRFVelocity;
        inletValue      uniform (0 0 0);
        relative        yes;
        value           uniform (0 0 0);    // initial value
    }
    \endverbatim

See also
    Foam::fixedValueFvPatchField

SourceFiles
    SRFVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_SRFVelocityFvPatchVectorField_H
#define Foam_SRFVelocityFvPatchVectorField_H

#include "fvPatchFields.H"
#include "fixedValueFvPatchFields.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
               Class SRFVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class SRFVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef SRFVelocityFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

    // Private Data

        //- Is the supplied inlet value relative to the SRF
        Switch relative_;

        //- Inlet value [m/s]
        vectorField inletValue_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "SRFVelocity");


    // Constructors

        //- Construct from patch and internal field
        SRFVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        SRFVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        SRFVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        SRFVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        SRFVelocityFvPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
        }


    // Member functions

        // Access

            //- Is supplied inlet value relative to the SRF?
            bool relative() const
            {
                return relative_;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchVectorField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        // I-O

            //- Write
            virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
