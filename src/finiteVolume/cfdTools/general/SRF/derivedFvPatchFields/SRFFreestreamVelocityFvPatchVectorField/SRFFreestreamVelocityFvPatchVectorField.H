/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2015 OpenCFD Ltd.
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
    Foam::SRFFreestreamVelocityFvPatchVectorField

Description
    Freestream velocity condition to be used in conjunction with the single
    rotating frame (SRF) model (see: SRFModel class)

    Given the free stream velocity in the absolute frame, the condition
    applies the appropriate rotation transformation in time and space to
    determine the local velocity using:

    \f[
        U_p = cos(\theta) U_{Inf} + sin(\theta) (n \times U_{Inf}) - U_{p,srf}
    \f]

    where
    \vartable
        U_p     | patch velocity [m/s]
        U_{Inf} | free stream velocity in the absolute frame [m/s]
        \theta  | swept angle [rad]
        n       | axis direction of the SRF
        U_{p,srf} | SRF velocity of the patch [m/s]
    \endvartable


Usage
    \table
        Property        | Description               | Required | Default value
        UInf            | freestream velocity       | yes      |
        relative        | UInf relative to the SRF? | no       |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            SRFFreestreamVelocity;
        UInf            uniform (0 0 0);
        relative        no;
        value           uniform (0 0 0);    // initial value
    }
    \endverbatim

See also
    Foam::freestreamFvPatchField
    Foam::SRFVelocityFvPatchVectorField

SourceFiles
    SRFFreestreamVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_SRFFreestreamVelocityFvPatchVectorField_H
#define Foam_SRFFreestreamVelocityFvPatchVectorField_H

#include "inletOutletFvPatchFields.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
           Class SRFFreestreamVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class SRFFreestreamVelocityFvPatchVectorField
:
    public inletOutletFvPatchVectorField
{
    typedef SRFFreestreamVelocityFvPatchVectorField this_bctype;
    typedef inletOutletFvPatchVectorField parent_bctype;

    // Private data

        //- Is the supplied inlet value relative to the SRF
        Switch relative_;

        //- Velocity of the free stream in the absolute frame [m/s]
        vector UInf_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "SRFFreestreamVelocity");


    // Constructors

        //- Construct from patch and internal field
        SRFFreestreamVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        SRFFreestreamVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        SRFFreestreamVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        SRFFreestreamVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        SRFFreestreamVelocityFvPatchVectorField(const this_bctype&) = delete;

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

            //- Return the velocity at infinity
            const vector& UInf() const
            {
                return UInf_;
            }

            //- Return reference to the velocity at infinity to allow adjustment
            vector& UInf()
            {
                return UInf_;
            }


        // Evaluation functions

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

#endif

// ************************************************************************* //
