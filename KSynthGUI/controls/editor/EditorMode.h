#ifndef EDITORMODE_H
#define EDITORMODE_H


/**
 * the editor may use several modes, changing its behaviour.
 * this enum lists all supported modes
 */
enum class EditorMode {

	/** modifiy existing entries (e.g. resize/move notes) */
	CURSOR,

	/** insert new entries (e.g. notes) */
	DRAW

};

#endif // EDITORMODE_H
