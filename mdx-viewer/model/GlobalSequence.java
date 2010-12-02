package org.aprog.mdxviewer.model;

public final class GlobalSequence {
	
	private final SequenceTimer sequenceTimer;
	
	public GlobalSequence(final int duration) {
		this.sequenceTimer = new SequenceTimer(0,duration);
	}
	
	final void updateTimer(final int dt) {
		this.sequenceTimer.update(dt);
	}
	
	final int getTime() {
		return this.sequenceTimer.getTime();
	}
	
}
