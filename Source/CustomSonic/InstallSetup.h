#pragma once

static inline boost::shared_ptr<hh::db::CDatabase> database;

class InstallSetup
{
public:
	static void applyPatches();
	static void applyPostPatches();
};