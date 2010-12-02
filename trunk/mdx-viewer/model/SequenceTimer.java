package org.aprog.mdxviewer.model;

import org.aprog.mdxviewer.util.Debug;

public final class SequenceTimer {
	
	private final int start, duration;
	
	private int time;
	
	public SequenceTimer(final int start, final int end) {
		Debug.checkInput(0<=start);
		Debug.checkInput(start<=end);
		this.start = start;
		this.duration = Math.max(1,end-start);
		this.time = start;
	}
	
	public final void update(final int dt) {
		Debug.checkInput(0<=dt);
		time = this.start+((this.time-this.start+dt)%this.duration);
	}

	public final int getTime() {
		return time;
	}
	
}
