/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2020-2021 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "patchInteractionData.H"
#include "dictionaryEntry.H"
#include "PatchInteractionModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const Foam::word Foam::patchInteractionData::beiGosmanTypeName("BaiGosman");


namespace
{
void validateBaiGosmanData
(
    const Foam::dictionary& dict,
    const Foam::patchInteractionData& pid
)
{
    if (pid.Tmelt() <= Foam::SMALL)
    {
        FatalIOErrorInFunction(dict)
            << "Tmelt must be > 0 for patch " << pid.patchName()
            << " when type is " << pid.beiGosmanTypeName
            << Foam::nl << Foam::exit(Foam::FatalIOError);
    }

    if (pid.Wec() <= Foam::SMALL)
    {
        FatalIOErrorInFunction(dict)
            << "Wec must be > 0 for patch " << pid.patchName()
            << " when type is " << pid.beiGosmanTypeName
            << Foam::nl << Foam::exit(Foam::FatalIOError);
    }

    if (pid.parcelsPerSplash() < 1)
    {
        FatalIOErrorInFunction(dict)
            << "parcelsPerSplash must be >= 1 for patch " << pid.patchName()
            << Foam::nl << Foam::exit(Foam::FatalIOError);
    }

    if (pid.Adry() <= Foam::SMALL)
    {
        FatalIOErrorInFunction(dict)
            << "Adry must be > 0 for patch " << pid.patchName()
            << Foam::nl << Foam::exit(Foam::FatalIOError);
    }

    if (pid.Awet() <= Foam::SMALL)
    {
        FatalIOErrorInFunction(dict)
            << "Awet must be > 0 for patch " << pid.patchName()
            << Foam::nl << Foam::exit(Foam::FatalIOError);
    }

    if (pid.Cf() < 0)
    {
        FatalIOErrorInFunction(dict)
            << "Cf must be >= 0 for patch " << pid.patchName()
            << Foam::nl << Foam::exit(Foam::FatalIOError);
    }

    if (pid.dMinSplash() > 0 && pid.dMaxSplash() > 0)
    {
        if (pid.dMinSplash() >= pid.dMaxSplash())
        {
            FatalIOErrorInFunction(dict)
                << "dMinSplash must be < dMaxSplash for patch "
                << pid.patchName()
                << Foam::nl << Foam::exit(Foam::FatalIOError);
        }
    }
}
} // End namespace


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

Foam::patchInteractionData::patchInteractionData()
:
    interactionTypeName_(),
    patchName_(),
    e_(1),
    mu_(0),
    dry_(true),
    parcelsPerSplash_(5),
    splashParcelType_(-1),
    deltaWet_(0),
    dMinSplash_(-1),
    dMaxSplash_(-1),
    Adry_(2630),
    Awet_(1320),
    Cf_(0.7),
    Tref_(-1),
    Tmelt_(-1),
    Wec_(-1)
{}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>
(
    Istream& is,
    patchInteractionData& pid
)
{
    is.check(FUNCTION_NAME);

    const dictionaryEntry dictEntry(dictionary::null, is);
    const dictionary& dict = dictEntry.dict();

    pid.patchName_ = dictEntry.keyword();

    dict.readEntry("type", pid.interactionTypeName_);
    pid.e_ = dict.getOrDefault<scalar>("e", 1);
    pid.mu_ = dict.getOrDefault<scalar>("mu", 0);

    if (pid.isBaiGosman())
    {
        dict.readEntry("dry", pid.dry_);
        dict.readEntry("Tmelt", pid.Tmelt_);
        dict.readEntry("Wec", pid.Wec_);

        pid.parcelsPerSplash_ =
            dict.getOrDefault<label>("parcelsPerSplash", 5);
        pid.deltaWet_ = dict.getOrDefault<scalar>("deltaWet", 0);
        pid.splashParcelType_ =
            dict.getOrDefault<label>("splashParcelType", -1);
        pid.dMinSplash_ = dict.getOrDefault<scalar>("dMinSplash", -1);
        pid.dMaxSplash_ = dict.getOrDefault<scalar>("dMaxSplash", -1);
        pid.Adry_ = dict.getOrDefault<scalar>("Adry", 2630);
        pid.Awet_ = dict.getOrDefault<scalar>("Awet", 1320);
        pid.Cf_ = dict.getOrDefault<scalar>("Cf", 0.7);
        pid.Tref_ = dict.getOrDefault<scalar>("Tref", -1);

        validateBaiGosmanData(dict, pid);
    }

    return is;
}


Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const patchInteractionData& pid
)
{
    os << pid.patchName_;
    os.beginBlock();
    os.writeEntry("type", pid.interactionTypeName_);
    os.writeEntry("e", pid.e_);
    os.writeEntry("mu", pid.mu_);

    if (pid.isBaiGosman())
    {
        os.writeEntry("dry", pid.dry_);
        os.writeEntry("Tmelt", pid.Tmelt_);
        os.writeEntry("Wec", pid.Wec_);
        os.writeEntry("parcelsPerSplash", pid.parcelsPerSplash_);
        os.writeEntry("deltaWet", pid.deltaWet_);
        os.writeEntry("splashParcelType", pid.splashParcelType_);
        os.writeEntry("dMinSplash", pid.dMinSplash_);
        os.writeEntry("dMaxSplash", pid.dMaxSplash_);
        os.writeEntry("Adry", pid.Adry_);
        os.writeEntry("Awet", pid.Awet_);
        os.writeEntry("Cf", pid.Cf_);
        os.writeEntry("Tref", pid.Tref_);
    }

    os.endBlock();

    os.check(FUNCTION_NAME);
    return os;
}


// ************************************************************************* //
