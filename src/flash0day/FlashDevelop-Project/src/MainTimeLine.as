package  
{
	import flash.display.*;
	import flash.system.*;
	
	/**
	 * ...
	 * @author cod
	 */
	dynamic public class MainTimeLine extends MovieClip 
	{
		public var d:Date;
		public var spl:Object;
		public function MainTimeLine() 
		{
			this.graphics.beginFill(0xff0000);
			this.graphics.drawRect(0, 0, 0, 0);
			addFrameScript(0, this.frame1);
			
			return;
		}
		
		internal function frame1() : void
		{
			this.d = new Date();
			
			//if (this.d.getTime() > new Date(2013, 3, 10).getTime())
			//{
			//	return;
			//}
			
			//if (this.d.getTime() < new Date(2013, 1, 1).getTime())
			//{
			//	return;
			//}
			
			if (Capabilities.version.indexOf("WIN 11") < 0)
			{
			//	return;
			}
			
			this.spl = new Rabbit  	();
			return;
		}
		
	}

}