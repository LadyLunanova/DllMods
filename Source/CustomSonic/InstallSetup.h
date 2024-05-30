#pragma once

inline boost::shared_ptr<hh::db::CDatabase> database_ui;
inline boost::shared_ptr<hh::db::CDatabase> database_sonic;

class InstallSetup
{
public:
	static void applyPatches();
	static void applyPostPatches();
};