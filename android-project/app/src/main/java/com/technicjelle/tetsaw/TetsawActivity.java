package com.technicjelle.tetsaw;

import org.libsdl.app.SDLActivity;

public class TetsawActivity extends SDLActivity {
	protected String[] getLibraries() {
		return new String[]{"SDL3", "Tetsaw"};
	}
}
