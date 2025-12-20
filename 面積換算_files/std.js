
	var appNetscape = (navigator.appName.toUpperCase().indexOf("NETSCAPE")!=-1);
	
	String.prototype.startsWith = function(str){return (this.match("^"+str)==str)}
	String.prototype.endsWith = function(str){return (this.match(str+"$")==str)}
	
	if(parent != null && parent.calendar_Show)
		var calendar_Show = parent.calendar_Show;
		
	if(parent != null && parent.colorPicker_Show)
		var colorPicker_Show = parent.colorPicker_Show;
		
	var calendar_Cancel;
	var colorPicker_Cancel;
	var clear_search;
	
	var objb;
	
	var isRow = false;
	var doKeyDown = true;
	var doMouseDown = true;
	document.onkeydown=kd;
	//document.onmousedown=md;
	
	var doBlur = null;
	var doChange = null;
	layerRef="document.all";
	styleSwitch=".style";
	
	var waitFooter;
	                                                                  
	//document.oncontextmenu=right_click;
	function right_click(){
		alert('請勿使用滑鼠右鍵!');     
		return false;
	}

	function changePageSubmit(notAlert) {
		/*
		if (document.all["action"].value!="") {
			alert(' 處理中......請稍待......');
			return false;
		}
		*/
			
		var o = getMsg();
		if (o == null) {
			if (!notAlert)
				alert(' 處理中......請稍待......');
			return false;
		}	
		if (o.innerHTML.indexOf("請稍待") != -1) {
			if (!notAlert)
				alert(' 處理中......請稍待......');
			return false;
		}
							
		try {
			if(calendar_Cancel == null && parent != null && parent.calendar_Cancel)
				calendar_Cancel = parent.calendar_Cancel;
			if(calendar_Cancel)
				calendar_Cancel();
		} 
		catch (e) {
			//alert(e.name + ":" + e.message);
		}
			
		try {
			if(colorPicker_Cancel == null && parent != null && parent.colorPicker_Cancel)
				colorPicker_Cancel = parent.colorPicker_Cancel;
			if(colorPicker_Cancel)
				colorPicker_Cancel();
		} 
		catch (e) {
			//alert(e.name + ":" + e.message);
		}
			
		try {
			if(clear_search == null && parent != null && parent.clear_search)
				clear_search = parent.clear_search;
				
			var bIsSearchGrid = false;
			try {
				if (hide_grid != null)
					bIsSearchGrid = true;
			} 
			catch (e) {
				//alert(e.name + ":" + e.message);
			}
			
			if(clear_search && !bIsSearchGrid)
				clear_search();
		} 
		catch (e) {
			//alert(e.name + ":" + e.message);
		}
			
		return true;
	}
	
	function kd(){
		var event = (window.event?window.event:kd.arguments[kd.arguments.length-1]);
		var srcObj = (event.srcElement?event.srcElement:event.target);
		//setMsg(event.keyCode+" kd:"+doKeyDown);
		
		if(!doKeyDown) {
			doKeyDown = true;
			return false;
		}
		else {
			if (event.keyCode == 13) {
				if (appNetscape) {
					/*
					event.returnValue = false;
					event.keyCode = 9;
				    var newEvent = document.createEvent("KeyboardEvent");
				    newEvent.initKeyEvent(event.type, event.bubbles, event.cancelable, event.view,
				                          event.ctrlKey, event.altKey, event.shiftKey,
				                          event.metaKey, event.keyCode, event.charCode);
				    event.preventDefault();
				    event.target.dispatchEvent(newEvent);
				    */
    					
					/* 暫時無解，nextSibling在<table>怪怪的
					// 不允許enter觸發按鈕，避免誤按
					//if(srcObj != null && srcObj.type != null && (srcObj.type.toUpperCase() == "BUTTON" || srcObj.type.toUpperCase() == "SUBMIT")) {
						//return true;
					//}
					
					var nextObj = srcObj;
					while(nextObj != null) {
						nextObj = nextObj.nextSibling;
						if(nextObj != null && nextObj.type != null) {
							try {
								setTextRange(srcObj, nextObj, event);
								nextObj.focus();
								return false;
							} 
							catch (e) {
								//alert(e.name + ":" + e.message);
							}
						}
					}
					*/
				}
				else {
					if (srcObj == null || srcObj.type == null || srcObj.type.toUpperCase() != "TEXTAREA")
						event.keyCode = 9;
				}
			}
			/* event.ctrlKey = false 有問題不能用
			else if (event.ctrlKey == true) {
				if (event.keyCode == 37) {	
					if (appNetscape) {
						// 暫時無解，nextSibling在<table>怪怪的
					}
					else {
						event.ctrlKey = false;
						event.shiftKey = true;
						event.keyCode = 9;
					}
				}
				else if (event.keyCode == 39) {
					if (appNetscape) {
						// 暫時無解，nextSibling在<table>怪怪的
					}
					else {
						event.ctrlKey = true;
						event.keyCode = 9;
					}
				}
			}
			*/
			
			return true;
		}	
	}
	
	function md(){	
	//setMsg(findObj("action"));
	/*
	var event = (window.event?window.event:md.arguments[md.arguments.length-1]);
	var srcObj = (event.srcElement?event.srcElement:event.target);
	setMsg(srcObj.tagName);
	*/
		if(!doMouseDown) {
			doMouseDown = true;
			return false;
		}
		else {
			return true;
		}	
	}
	
	//讓 Chrome、firefox 支援 outerhtml
	if (window.HTMLElement) {
		var element = HTMLElement.prototype;
		if (element.__defineGetter__) {
			element.__defineGetter__("outerHTML",
				function () {
					var div = document.createElement('div'), h;
					div.appendChild( this.cloneNode(true) );
					h = div.innerHTML;
					div = null;
					return h;
				}
			);
		}
		if (element.__defineSetter__) {
			element.__defineSetter__("outerHTML",
				function (s) {
					var range = this.ownerDocument.createRange();
					range.setStartBefore(this);
					var fragment = range.createContextualFragment(s);
					//alert(fragment.xml);
					this.parentNode.replaceChild(fragment, this);
				}
			);
		}
	}
	
	function divWindow(n, f) {
		var o = findObj(n);
		if (o == null) {
			var div = document.createElement('div');
			document.body.insertBefore(div, document.body.childNodes[0]);
			div.innerHTML = "<DIV style=\"position:absolute;left:-9999px;top:-9999px; visibility:hidden; z-index:5\"><IFRAME NAME=\""+n+"\" SRC=\"about:blank\"></IFRAME></DIV>";
			o = findObj(n);
		}
		if (o == null)
			window.open(f);
		else {
			var pFreme = findObj('parent.'+self.name);
			if (pFreme != null) {
				try {
					pFreme.onload();
				} 
				catch (e) {
					//alert(e.name + ":" + e.message);
				}
			}
			o.src = f;
		}
		return true;
	}
	
	function download(f) {
		return divWindow("downloadiframe", "downLoadFile/"+f);
	}
		
	function getMsg(){
		var o = findObj("msg");
		if (o == null) {
			o = findObj("message.msg");
		}
		if (o == null) {
			o = findObj("parent.msg");
		}
		if (o == null) {
			o = findObj("parent.message.msg");
		}
		if (o == null) {
			o = findObj("parent.parent.msg");
		}
		if (o == null) {
			o = findObj("parent.parent.message.msg");
		}
		if (o == null) {
			o = findObj("parent.parent.parent.msg");
		}
		if (o == null) {
			o = findObj("parent.parent.parent.message.msg");
		}
		return o;
	}
	function setMsg(msg, withAlert){
		var o = getMsg();
			
		if (o == null) {
			alert(msg);
		}
		else {
			o.innerHTML = msg;
			if (withAlert)
				alert(msg);
		}
		return true;
	}
	function getState(){
		var o = findObj("state");
		if (o == null) {
			o = findObj("message.state");
		}
		if (o == null) {
			o = findObj("parent.state");
		}
		if (o == null) {
			o = findObj("parent.message.state");
		}
		if (o == null) {
			o = findObj("parent.parent.state");
		}
		if (o == null) {
			o = findObj("parent.parent.message.state");
		}
		if (o == null) {
			o = findObj("parent.parent.parent.state");
		}
		if (o == null) {
			o = findObj("parent.parent.parent.message.state");
		}
		return o;
	}
	function setState(state){
		var o = getState();
		if (o == null) {
			//alert(state);
		}
		else {
			o.innerHTML = state;
		}
		return true;
	}
		
	function changeBtnColor(name,color){
		if (eval(layerRef+'["'+name+'"]') != null && !eval(layerRef+'["'+name+'"].disabled'))
			eval(layerRef+'["'+name+'"]'+styleSwitch+'.color=color');
	}	

	var text_onkeyup_obj_out;
	var text_onkeyup_before_out;
	var text_onkeyup_changeF_out;
	
	var text_onkeyup_obj_in;
	var text_onkeyup_before_in;
	var text_onkeyup_changeF_in;
	
	function text_onchange(t, changeF) {
		//console.log('t:'+t.name+' text_onchange changeF:'+changeF);
		text_onkeyup_obj_out = text_onkeyup_obj_in;
		text_onkeyup_before_out = text_onkeyup_before_in;
		text_onkeyup_changeF_out = text_onkeyup_changeF_in;
		
		text_onkeyup_obj_in = t;
		text_onkeyup_before_in = t.value;
		text_onkeyup_changeF_in = changeF;
	}
	
	function check_doChange(t, changecheck) {
		//console.log('t:'+t.name+' text_onkeyup_obj_in:'+(text_onkeyup_obj_in!=null?text_onkeyup_obj_in.name:text_onkeyup_obj_in));
		if (text_onkeyup_obj_in != null) {			
			if (t == text_onkeyup_obj_out) {// direct into next input
				//console.log('t:'+t.name+' text_onkeyup_obj_out');
				if (t.value != text_onkeyup_before_out) {
					//console.log('text_onkeyup_obj_out doChange');
					doChange = text_onkeyup_changeF_out;
				}
				else {
					//console.log('set doChange = null');
					doChange = null;
				}
				text_onkeyup_obj_out = null;
			}
			else if (t == text_onkeyup_obj_in) {
				//console.log('t:'+t.name+' text_onkeyup_obj_in');	
				if (t.value != text_onkeyup_before_in) {
					//console.log('text_onkeyup_obj_in doChange');
					doChange = text_onkeyup_changeF_in;
				}
				else {
					//console.log('set doChange = null');
					doChange = null;
				}
				text_onkeyup_obj_in = null;
				text_onkeyup_obj_out = null;
			}
			else {
				//console.log('t:'+t.name+' no match');
			}				
		}
		
		//console.log('check doChange:'+doChange);
		if (doChange != null) {
			if (changecheck == null)
				changecheck = 1;
				
			if ((changecheck == 1 || (changecheck == 2 && findObj("r").value == "F") || (changecheck == 3 && findObj("r").value != "F")) && eval(doChange)==false) {
				doChange = null;
				return false;
			}
			
			doChange = null;
		}
		
		return true;
	}		
	
	//onkeyup= 8[back] 9[] 13[enter] 16[shift] 17[ctrl] 18[alt] 19[pause] 20[caps] 27[esc] 33[page up] 34[page dn] 35[end] 36[home] 37[left] 38[up] 39[right] 40[down] 45[ins] 46[del]
	//text|choice，帶出符合highlight select
	function text_onfind(t, b, upper, check) {
			
		var event = (window.event?window.event:text_onfind.arguments[text_onfind.arguments.length-1]);
		//alert(event.keyCode);
	
		if (upper != null && upper && text_onfind.arguments.length > 3)
			text_upperCase(t);
			
		if (b == null)
			b = false;
			
		if (check == null)
			check = true;	
			
		var tmp = "";
		var st = t.value;	
		if (findObj("r").value == "F") {
			if (t.value.indexOf(">=") == 0 || t.value.indexOf("<=") == 0) {
				tmp = st.substring(0, 2);
				st = t.value.substring(2);
			}
			else if (t.value.indexOf("~") == 0 || t.value.indexOf("?") == 0 || t.value.indexOf("=") == 0 || t.value.indexOf(">") == 0 || t.value.indexOf("<") == 0) {
				tmp = st.substring(0, 1);
				st = t.value.substring(1);
			}
			
			var lastp = st.lastIndexOf(",");
			if (lastp != -1) {
				tmp = tmp + st.substring(0, lastp+1);
				st = st.substring(lastp+1);
			}
		}
	
		if ((b || findObj("r").value != "F") && check) {
			doMouseDown = false;			
			
			var sfind = "";
			var c = findObj(t.name+"check");
			if (c != null) {			
				if (c[0]!=null && c[0].name!=null) {
					for (var iTemp = 0; iTemp<c.length; iTemp++) {
						if(st=="" || event.keyCode == 8 || event.keyCode == 46) {
							c[iTemp].selectedIndex = 0;
						}
						else if(event.keyCode == 9 || event.keyCode == 16 || event.keyCode == 17 || event.keyCode == 18 || event.keyCode == 19 || event.keyCode == 20 || event.keyCode == 27 || event.keyCode == 33 || event.keyCode == 34 || event.keyCode == 35 || event.keyCode == 36 || event.keyCode == 37 || event.keyCode == 38 || event.keyCode == 39 || event.keyCode == 40 || event.keyCode == 45) {
						}
						else {
							var bFind = false;
							for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
								if (c[iTemp].options[cIndex].value.indexOf(st) == 0) {
									bFind = true;
									sfind = c[iTemp].options[cIndex].value.substring(st.length);
									//if (sfind != "")
										t.value = tmp + st + sfind;
									c[iTemp].selectedIndex = cIndex;
									break;
								}	
							}
							if (!bFind) {
								var stLastCase = st.substring(st.length-1);
								st = st.substring(0, st.length-1) + stLastCase.toUpperCase();
								for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
									if (c[iTemp].options[cIndex].value.indexOf(st) == 0) {
										bFind = true;
										sfind = c[iTemp].options[cIndex].value.substring(st.length);
										//if (sfind != "")
											t.value = tmp + st + sfind;
										c[iTemp].selectedIndex = cIndex;
										break;
									}	
								}
							}
							if (!bFind) {
								var stLastCase = st.substring(st.length-1);
								st = st.substring(0, st.length-1) + stLastCase.toLowerCase();
								for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
									if (c[iTemp].options[cIndex].value.indexOf(st) == 0) {
										bFind = true;
										sfind = c[iTemp].options[cIndex].value.substring(st.length);
										//if (sfind != "")
											t.value = tmp + st + sfind;
										c[iTemp].selectedIndex = cIndex;
										break;
									}	
								}
							}
							if (!bFind)
								c[iTemp].selectedIndex = 0;
						}
					}
				}
				else {
					if(st=="" || event.keyCode == 8 || event.keyCode == 46) {
						c.selectedIndex = 0;
					}	
					else if(event.keyCode == 9 || event.keyCode == 16 || event.keyCode == 17 || event.keyCode == 18 || event.keyCode == 19 || event.keyCode == 20 || event.keyCode == 27 || event.keyCode == 33 || event.keyCode == 34 || event.keyCode == 35 || event.keyCode == 36 || event.keyCode == 37 || event.keyCode == 38 || event.keyCode == 39 || event.keyCode == 40 || event.keyCode == 45) {
					}
					else {
						var bFind = false;
						for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
							if (c.options[cIndex].value.indexOf(st) == 0) {
								bFind = true;
								sfind = c.options[cIndex].value.substring(st.length);
								//if (sfind != "")		
									t.value = tmp + st + sfind;
								c.selectedIndex = cIndex;
								break;
							}	
						}
						if (!bFind) {
							var stLastCase = st.substring(st.length-1);
							st = st.substring(0, st.length-1) + stLastCase.toUpperCase();
							for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
								if (c.options[cIndex].value.indexOf(st) == 0) {
									bFind = true;
									sfind = c.options[cIndex].value.substring(st.length);
									//if (sfind != "")		
										t.value = tmp + st + sfind;
									c.selectedIndex = cIndex;
									break;
								}	
							}
						}
						if (!bFind) {
							var stLastCase = st.substring(st.length-1);
							st = st.substring(0, st.length-1) + stLastCase.toLowerCase();
							for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
								if (c.options[cIndex].value.indexOf(st) == 0) {
									bFind = true;
									sfind = c.options[cIndex].value.substring(st.length);
									//if (sfind != "")		
										t.value = tmp + st + sfind;
									c.selectedIndex = cIndex;
									break;
								}	
							}
						}
						if (!bFind)
							c.selectedIndex = 0;
					}
				}
			}
			
			if(t.value != "" && (event.keyCode == 13 || (event.ctrlKey == true && (event.keyCode == 37 || event.keyCode == 38 || event.keyCode == 39 || event.keyCode == 40)))) {
				sfind = t.value;
			}

			//alert(sfind);
			if (sfind != "") {
				try {
					if (t != null && t.type != null && (t.type.toUpperCase() == "PASSWORD" || t.type.toUpperCase() == "TEXT" || t.type.toUpperCase() == "TEL" || t.type.toUpperCase() == "TEXTAREA")) {
						if (appNetscape) {
							t.setSelectionRange(t.value.length-sfind.length, t.value.length); 
						}
						else {
							var range = t.createTextRange();
							if (range) {
								range.moveStart('character', t.value.length-sfind.length);						
								range.moveEnd('character', t.value.length);						
								range.select();
							}
						}
					}
				} 
				catch (e) {
					//alert(e.name + ":" + e.message);
				}
			}
			
			if (c != null) {
				var bFindKeyWord = false;	
				var textSelect_list_temp = new Array();
				var textSelect_list_new = new Array();
				
				if (c[0]!=null && c[0].name!=null) {
						
					for (var iTemp = 0; iTemp<c.length; iTemp++) {
						if (c[iTemp].selectedIndex == 0) {
							textSelect_list_temp[iTemp] = new Array;
							if (textSelect_list == null) {
								textSelect_list_temp[iTemp] = new Array;
								for (var cIndex = 0; cIndex<c[iTemp].options.length; cIndex++)
									textSelect_list_temp[iTemp][textSelect_list_temp[0].length] = c[iTemp].options[cIndex];
							}
							else
								textSelect_list_temp = textSelect_list;
							
							for (var j = 0; j<textSelect_list_temp[iTemp].length; j++) {
								if (t.value != "" && textSelect_list_temp[iTemp][j].text.indexOf(t.value) != -1) {
									bFindKeyWord = true;
									textSelect_list_new[iTemp][textSelect_list_new[iTemp].length] = textSelect_list_temp[iTemp][j];
								}
							}
							
							if (textSelect_list_new[iTemp].length > 0) {
								c[iTemp].options.length = 0;
								for (var j = 0; j<textSelect_list_new[iTemp].length; j++)
									c[iTemp].add(textSelect_list_new[iTemp][j], j+1);
								var optionCount = new Option(c[iTemp].options.length+" 筆符合", "");
								c[iTemp].add(optionCount, 0);
							}
							else if (c[iTemp].options[0].text.endsWith("筆符合")) {
								c[iTemp].options.length = 0;
								for (var j = 0; j<textSelect_list_temp[iTemp].length; j++)
									c[iTemp].add(textSelect_list_temp[iTemp][j], j);
							}
							c[iTemp].selectedIndex = 0;
						}
					}
				}
				else {
					if (c.selectedIndex == 0) {
						textSelect_list_new[0] = new Array;
						if (textSelect_list == null) {
							textSelect_list_temp[0] = new Array;
							for (var cIndex = 0; cIndex<c.options.length; cIndex++)
								textSelect_list_temp[0][textSelect_list_temp[0].length] = c.options[cIndex];
						}
						else
							textSelect_list_temp = textSelect_list;
						
						for (var j = 0; j<textSelect_list_temp[0].length; j++) {
					//console.log(textSelect_list_temp[0][j].text);
							if (t.value != "" && textSelect_list_temp[0][j].text.indexOf(t.value) != -1) {
								bFindKeyWord = true;
								textSelect_list_new[0][textSelect_list_new[0].length] = textSelect_list_temp[0][j];
							}
						}
						
						if (textSelect_list_new[0].length > 0) {
							c.options.length = 0;
							for (var j = 0; j<textSelect_list_new[0].length; j++)
								c.add(textSelect_list_new[0][j], j+1);
							var optionCount = new Option(c.options.length+" 筆符合", "");
							c.add(optionCount, 0);
							textSelect_filter = t.value;
							//console.log(textSelect_filter);
						}
						else if (c.options[0].text.endsWith("筆符合")) {
							c.options.length = 0;
							for (var j = 0; j<textSelect_list_temp[0].length; j++)
								c.add(textSelect_list_temp[0][j], j);
						}
						c.selectedIndex = 0;
					}
				}
						
				if (bFindKeyWord && textSelect_list == null) {
					//console.log(bFindKeyWord);
					textSelect_list = textSelect_list_temp;
				}					
			}
		}
		
		return true;
	}
	
	function text_onkeyup(t, l, n, b, upper, check) {
		var event = (window.event?window.event:text_onkeyup.arguments[text_onkeyup.arguments.length-1]);
	
		if (upper != null && upper)
			text_upperCase(t);
			
		if (b == null)
			b = false;
			
		if (check == null)
			check = true;
			
		if (!(event.keyCode == 9 || event.keyCode == 16 || event.keyCode == 17 || event.keyCode == 18 || event.keyCode == 19 || event.keyCode == 20 || event.keyCode == 27 || event.keyCode == 33 || event.keyCode == 34 || event.keyCode == 35 || event.keyCode == 36 || event.keyCode == 37 || event.keyCode == 38 || event.keyCode == 39 || event.keyCode == 40 || event.keyCode == 45)) {
	
			if (l != null && n != null && t.value.length >= l) {
				if ((b || findObj("r").value != "F") && check) {
					doMouseDown = false;
					findObj(n).focus();
				}
			}
		}
		
		return true;
	}

	function changeNum(st) {
		var bn = (st.charAt(0) == "-");
		if (bn)
			st = st.substring(1);
		while (st.charAt(0) == "0" && st.length > 1)
			st = st.substring(1);
			
		return parseInt((bn?"-"+st:st));
	}
	
	function text_upperCase(t) {
		var temp = t.value.toUpperCase();
		if (t.value != temp)
			t.value = temp;
		return true;
	}
	
	function doCheck_onblur(t) {
		if (!appNetscape && doBlur != null && doBlur != t) {
			doChange = null;
			return false;
		}		
		doBlur = t;
		return true;
	}
	
	function text_onblur_changeColor(t, n, b, upper, check, changecheck) {		
		if (!doCheck_onblur(t))
			return;
		
		if (upper != null && upper)
			text_upperCase(t);
			
		var st = t.value;
			
		if (b == null)
			b = false;			
			
		if (check == null)
			check = true;
	
		var isColor = checkColor(t.value);
		var id = findObj(t.name+".crid");
		if (st != "") {
			if ((b || findObj("r").value != "F") && check) {
				if (!isColor) {	
					id.style.background="white";
					setCheckNoPass(n + " 顏色輸入錯誤!", t);
					return;
				}
			}
		}
		
		if (isColor) {
			id.style.background=st;
		}
		else {
			id.style.background="white";
		}
		
		if (!check_doChange(t, changecheck))
			return;
		
		doBlur = null;
		
		return true;
	}

	function text_onblur_checkDate(t, n, s, e, taiwandate, dp, b, upper, check, changecheck) {
		if (!doCheck_onblur(t))
			return;
		
		if (upper != null && upper)
			text_upperCase(t);
		
		var bpass = true;
		var st = t.value;
	
		if (taiwandate == null)
			taiwandate = false;
			
		if (dp == null)
			dp = "/";
			
		if (b == null)
			b = false;
			
		if (check == null)
			check = true;
	
		var sttmp = "";
		if (st != "") {
			var tmp = "";
			if (st.indexOf(">=") == 0 || st.indexOf("<=") == 0) {
				tmp = st.substring(0, 2);
				st = st.substring(2);
			}
			else if (st.indexOf("~") == 0 || st.indexOf("?") == 0 || st.indexOf("=") == 0 || st.indexOf(">") == 0 || st.indexOf("<") == 0) {
				tmp = st.substring(0, 1);
				st = st.substring(1);
			}			
			
			if ((b || findObj("r").value != "F") && check) {					
				if (tmp != "" || st.indexOf(",") != -1) {
					setCheckNoPass(n + " 日期輸入錯誤!", t);
					return;
				}
			}
				
			sttmp += tmp;
				
			var sts = -1;
			var ste = st.indexOf(",", sts+1);
			var stt = st;
			if (ste != -1)
				stt = st.substring(sts+1, ste);
			else	
				stt = st.substring(sts+1);
				
			while (stt != "") {
				var t1 = "";
				var t2 = "";
				var t3 = "";
				var p1 = -1;
				var p2 = -1;
				
				if (dp != "") {
					p1 = stt.indexOf(dp);
					p2 = -1;
					if (p1 != -1) {
						t1 = stt.substring(0, p1);
						if (p1 != stt.length-1) {
							t2 = stt.substring(p1+1);
							p2 = t2.indexOf(dp);
							if (p2 != -1) {
								if (p2 != t2.length-1) {
									t3 = t2.substring(p2+1);						
								}
								t2 = t2.substring(0, p2);
							}
							else {							
								if (t2.length > 2) {
									t3 = t2.substring(2);
									t2 = t2.substring(0, 2);
								}
							}
						}
					}
					else {
						var ys = 4;
						if (taiwandate)
							ys = 3;
						if (stt.charAt(0) == "-")
							ys++;
							
						if (stt.length <= ys) {
							t1 = stt;
						}
						else {
							t1 = stt.substring(0, ys);
							t2 = stt.substring(ys);
							p2 = t2.indexOf(dp);
							if (p2 != -1) {
								if (p2 != t2.length-1) {
									t3 = t2.substring(p2+1);						
								}
								t2 = t2.substring(0, p2);
							}
							else {							
								if (t2.length > 2) {
									t3 = t2.substring(2);
									t2 = t2.substring(0, 2);
								}
							}
						}	
					}
				}
				else {
					var ys = 4;
					if (taiwandate)
						ys = 3;
					if (stt.charAt(0) == "-")
						ys++;
						
					if (stt.length <= ys) {
						t1 = stt;
					}
					else {
						t1 = stt.substring(0, ys);
						t2 = stt.substring(ys);
						if (t2.length > 2) {
							t3 = t2.substring(2);
							t2 = t2.substring(0, 2);
						}
					}		
				}
				
				if (t1 != "" && dp == "") {
					var t0;
					if (!isNaN(t1)) {
						t0 = changeNum(t1);						
						if (taiwandate) {
							if (t0 < -99)
								t1 = t0.toString();
							else if (t0 < -9)
								t1 = "-0"+t0.toString().substring(1);
							else if (t0 < 0)
								t1 = "-00"+t0.toString().substring(1);
							else if (t0 < 10)
								t1 = "00"+t0.toString();
							else if (t0 < 100)
								t1 = "0"+t0.toString();
							else
								t1 = t0.toString();
						}
						else {
							if (t0 < 10)
								t1 = "000"+t0.toString();
							else if (t0 < 100)
								t1 = "00"+t0.toString();
							else if (t0 < 1000)
								t1 = "0"+t0.toString();
							else
								t1 = t0.toString();
						}
					}
				}
				
				if (t2 != "") {
					var t0;
					if (!isNaN(t2)) {
						t0 = changeNum(t2);						
						if (taiwandate) {
							if (t0 < 10)
								t2 = "0"+t0.toString();
							else
								t2 = t0.toString();
						}
						else {
							if (t0 < 10)
								t2 = "0"+t0.toString();
							else
								t2 = t0.toString();
						}
					}
				}
				
				if (t3 != "") {
					var t0;
					if (!isNaN(t3)) {
						t0 = changeNum(t3);						
						if (taiwandate) {
							if (t0 < 10)
								t3 = "0"+t0.toString();
							else
								t3 = t0.toString();
						}
						else {
							if (t0 < 10)
								t3 = "0"+t0.toString();
							else
								t3 = t0.toString();
						}
					}
				}			
			
				if ((b || findObj("r").value != "F") && parent.calendar_CheckDate == null && check)
					bpass = parent.parent.calendar_CheckDate(t1 + dp + t2 + dp + t3, s, e, taiwandate, dp);
				else if ((b || findObj("r").value != "F") && check)
					bpass = parent.calendar_CheckDate(t1 + dp + t2 + dp + t3, s, e, taiwandate, dp);
					
				if (!bpass) {
					setCheckNoPass(n + " 日期輸入錯誤!", t);
					return;
				}
				
				if (dp == "") {
					sttmp += t1 + t2 + t3;
				}
				else {
					if (t2 != "" && t3 != "")
						sttmp += t1 + dp + t2 + dp + t3;
					else if (t2 != "")
						sttmp += t1 + dp + t2;
					else
						sttmp += t1;
				}
				
				if (ste != -1) {
					sttmp += ",";
					sts = st.indexOf(",", sts+1);
					ste = st.indexOf(",", sts+1);
					if (ste != -1)
						stt = st.substring(sts+1, ste);
					else	
						stt = st.substring(sts+1);
				}
				else {
					stt = "";	
				}				
			}
		}
		
		if (sttmp != "")
			t.value = sttmp;
		
		if (!check_doChange(t, changecheck))
			return;
		
		doBlur = null;
		
		return true;
	}	
	
	function checkMail(t) {
		if (t=="")
			return true;
		var a = new Array();
		a = t.split('@');
		if(a.length != 2) {
			return false;
		}
		for (var i=0; i < a.length; i++) {
			if(a[i]=="")
				return false;
		}
		
		var s = new Array();
		s = a[1].split('.');
		if(s.length < 2) {
			return false;
		}
		for (var i=0; i < s.length; i++) {
			if(s[i]=="")
				return false;
		}
		return true;
	}
	
	function formatNum(n) {
		var c1 = n.toString();
		var c2 = "";
		var p = 0;
		for (var i=c1.length-1; i>0-1; i--) {
			if (c2.length != 0 && (c2.length-p)%3 == 0) {
				c2 = ","+c2;
				p++;
			}
			c2 = c1.charAt(i)+c2;
		}
		return c2;
	}
	
	function addCommas(nStr, pZero) {
		nStr += '';
		x = nStr.split('.');
		
		var x1 = x[0];//整數部分
		var rgx = /(\d+)(\d{3})/;
		while (rgx.test(x1)) {
			x1 = x1.replace(rgx, '$1' + ',' + '$2');
		}
		
		var x2 = '';//小數部分		
		if (x.length > 1) {
			x2 = x[1];
		}

		if (pZero != null) {
			while (x2.length < pZero) {
				x2 = x2 + '0';
			}
		}
		
		if (x.length > 1 || pZero != null) {
			x2 = '.' + x2;
		}
		
		return x1 + x2;
	}

	function checkNum(v, i, d, b, f) {
	
		var n;
		var bN;
		
		if (isNaN(v)) {
			bN = (v.indexOf("-") != -1);
			var b1 = false;
			var b2 = false;
			for (var iTemp=0;iTemp<v.length;iTemp++) {
				cIndex = v.charAt(iTemp);
				
				if (cIndex == ".") {
					if (b1)
						return ".";
					else
						if (iTemp==0 || iTemp==v.length-1)
							return ".";
						else	
							b1 = true;
				}
				else if (cIndex == "-") {
					if (b2)
						return "-";
					else
						if (iTemp!=0)
							return "-";
						else	
							b2 = true;
				}
				else if (isNaN(cIndex) && cIndex != ",") {
					if (f != null && f.indexOf(cIndex) != -1) {
						v = v.substring(0, iTemp)+v.substring(iTemp+1);
						iTemp--;
					}
					else
						return cIndex;
				}
			}
			
				
			i1 = v.length;
			var iTempLangth = i1;
			if (bN)
				iTempLangth = i1-1;
			
			var ip = v.indexOf(".");
			if (ip == -1)
				ip = 0;
			else
				ip = i1-ip;
				
			for (var iTemp=0;iTemp<iTempLangth;iTemp++) {
				cIndex = v.charAt(i1-iTemp-1);
				if (cIndex == ",") {
					if (iTemp !=0 && (iTemp-ip)%3 == 0 && iTemp!=i1-1 && iTemp!=v.length-1) {
						v = v.substring(0, i1-iTemp-1)+v.substring(i1-iTemp);
						iTemp--;
						i1--;
					}
					else {
						return ",";
					}	
				}
			}			
			
			n = parseFloat(v);
		}
		else {
			bN = (v < 0);
			n = v;
		}
		
		var nc = 100000000000000;
		var ns = 10;

		if (bN) {
			nc = -nc;
			ns = -ns;
		}

		while (n/ns>1) {
			nc /= 10;
			ns *= 10;
		}	
		
		n = Math.round(n*nc)/nc;

		if (i != null) {
			var ni = 1;
			for (var iTemp=0;iTemp<i;iTemp++)
				ni *= 10;
				
			if (bN) {
				ni = -ni;
			}
		
			if (n/ni >= 1)
				return "I" + i;
		}

		if (d != null) {
			var nd = 1;
			for (var iTemp=0;iTemp<d;iTemp++)
				nd *= 10;
							
			nc = 100000000000000;
			ns = 10;
			
			if (bN) {
				nc = -nc;
				ns = -ns;
			}
		
			while (n*nd/ns>1) {
				nc /= 10;
				ns *= 10;
			}
			var nt = Math.round(n*nd*nc)/nc;	
			
			if (nt%1 != 0) {
				if (b != null && (b == "ceil" || b == "+")) {
					if (bN)
						return Math.ceil(-n*nd)/-nd;
					else
						return Math.ceil(n*nd)/nd;	
				}
				else if (b != null && (b == "floor" || b == "-")) {
					if (bN)
						return Math.floor(-n*nd)/-nd;
					else
						return Math.floor(n*nd)/nd;	
				}
				else if (b != null && (b == "round" || b)) {
					if (bN)
						return Math.round(-n*nd)/-nd;
					else
						return Math.round(n*nd)/nd;	
				}
				else
					return "D" + d;
			}
		}
		
		return n;
		
	}

	!function(e,t){"object"==typeof exports&&"undefined"!=typeof module?t():"function"==typeof define&&define.amd?define(t):t()}(0,function(){"use strict";function e(e){var t=this.constructor;return this.then(function(n){return t.resolve(e()).then(function(){return n})},function(n){return t.resolve(e()).then(function(){return t.reject(n)})})}function t(e){return new this(function(t,n){function o(e,n){if(n&&("object"==typeof n||"function"==typeof n)){var f=n.then;if("function"==typeof f)return void f.call(n,function(t){o(e,t)},function(n){r[e]={status:"rejected",reason:n},0==--i&&t(r)})}r[e]={status:"fulfilled",value:n},0==--i&&t(r)}if(!e||"undefined"==typeof e.length)return n(new TypeError(typeof e+" "+e+" is not iterable(cannot read property Symbol(Symbol.iterator))"));var r=Array.prototype.slice.call(e);if(0===r.length)return t([]);for(var i=r.length,f=0;r.length>f;f++)o(f,r[f])})}function n(e){return!(!e||"undefined"==typeof e.length)}function o(){}function r(e){if(!(this instanceof r))throw new TypeError("Promises must be constructed via new");if("function"!=typeof e)throw new TypeError("not a function");this._state=0,this._handled=!1,this._value=undefined,this._deferreds=[],l(e,this)}function i(e,t){for(;3===e._state;)e=e._value;0!==e._state?(e._handled=!0,r._immediateFn(function(){var n=1===e._state?t.onFulfilled:t.onRejected;if(null!==n){var o;try{o=n(e._value)}catch(r){return void u(t.promise,r)}f(t.promise,o)}else(1===e._state?f:u)(t.promise,e._value)})):e._deferreds.push(t)}function f(e,t){try{if(t===e)throw new TypeError("A promise cannot be resolved with itself.");if(t&&("object"==typeof t||"function"==typeof t)){var n=t.then;if(t instanceof r)return e._state=3,e._value=t,void c(e);if("function"==typeof n)return void l(function(e,t){return function(){e.apply(t,arguments)}}(n,t),e)}e._state=1,e._value=t,c(e)}catch(o){u(e,o)}}function u(e,t){e._state=2,e._value=t,c(e)}function c(e){2===e._state&&0===e._deferreds.length&&r._immediateFn(function(){e._handled||r._unhandledRejectionFn(e._value)});for(var t=0,n=e._deferreds.length;n>t;t++)i(e,e._deferreds[t]);e._deferreds=null}function l(e,t){var n=!1;try{e(function(e){n||(n=!0,f(t,e))},function(e){n||(n=!0,u(t,e))})}catch(o){if(n)return;n=!0,u(t,o)}}var a=setTimeout;r.prototype["catch"]=function(e){return this.then(null,e)},r.prototype.then=function(e,t){var n=new this.constructor(o);return i(this,new function(e,t,n){this.onFulfilled="function"==typeof e?e:null,this.onRejected="function"==typeof t?t:null,this.promise=n}(e,t,n)),n},r.prototype["finally"]=e,r.all=function(e){return new r(function(t,o){function r(e,n){try{if(n&&("object"==typeof n||"function"==typeof n)){var u=n.then;if("function"==typeof u)return void u.call(n,function(t){r(e,t)},o)}i[e]=n,0==--f&&t(i)}catch(c){o(c)}}if(!n(e))return o(new TypeError("Promise.all accepts an array"));var i=Array.prototype.slice.call(e);if(0===i.length)return t([]);for(var f=i.length,u=0;i.length>u;u++)r(u,i[u])})},r.allSettled=t,r.resolve=function(e){return e&&"object"==typeof e&&e.constructor===r?e:new r(function(t){t(e)})},r.reject=function(e){return new r(function(t,n){n(e)})},r.race=function(e){return new r(function(t,o){if(!n(e))return o(new TypeError("Promise.race accepts an array"));for(var i=0,f=e.length;f>i;i++)r.resolve(e[i]).then(t,o)})},r._immediateFn="function"==typeof setImmediate&&function(e){setImmediate(e)}||function(e){a(e,0)},r._unhandledRejectionFn=function(e){void 0!==console&&console&&console.warn("Possible Unhandled Promise Rejection:",e)};var s=function(){if("undefined"!=typeof self)return self;if("undefined"!=typeof window)return window;if("undefined"!=typeof global)return global;throw Error("unable to locate global object")}();"function"!=typeof s.Promise?s.Promise=r:s.Promise.prototype["finally"]?s.Promise.allSettled||(s.Promise.allSettled=t):s.Promise.prototype["finally"]=e});

	var textSelect;
	var textSelect_onblur;
	var textSelect_list;
	var textSelect_filter;
	function text_onblur(t, n, i, d, b, upper, check, changecheck) {
		if (!appNetscape)
			return text_onblur_timeout(t, n, i, d, b, upper, check, changecheck);			
		var p1 = new Promise( function (resolve, reject) {	
			setTimeout(function(t, n, i, d, b, upper, check, changecheck) {
				var tot = text_onblur_timeout(t, n, i, d, b, upper, check, changecheck);
				//console.log('tot:'+tot);
				var returnObj = new Array;
				returnObj[0] = (tot==true);
				returnObj[1] = t;
				resolve(returnObj);
				/*
				if (tot)
					resolve(t);
				else
					reject(t);
				*/
			}, 0, t, n, i, d, b, upper, check, changecheck);
		} );
		return p1;
	}
	function text_onblur_timeout(t, n, i, d, b, upper, check, changecheck) {
		if (!doCheck_onblur(t))
			return;
		
		if (upper != null && upper)
			text_upperCase(t);
		
		
		if (b == null)
			b = false;
			
		if (check == null)
			check = true;
			
		var c = findObj(t.name+"check");
		
		var doTextSelect_onblur = false;
		if (c != null) {
			if (c[0]!=null && c[0].name!=null) {
				for (var j = 0; j<c.length; j++) {
					if (c[j].name == document.activeElement.name) {
						doTextSelect_onblur = true;
						textSelect = c[j];
						textSelect_onblur = c[j].onblur;
						c[j].onblur = function() {
								text_onblur(t, n, i, d, b, upper, check, changecheck);
							};
						if (!c[j].options[0].text.endsWith("筆符合"))
							c[j].value = t.value;
						if (c[j].selectedIndex < 0)
							c[j].selectedIndex = 0;
					}
				}
			}
			else {
				if (c.name == document.activeElement.name) {
					doTextSelect_onblur = true;
					textSelect = c;
					textSelect_onblur = c.onblur;
					c.onblur = function() {
							text_onblur(t, n, i, d, b, upper, check, changecheck);
						};
					if (!c.options[0].text.endsWith("筆符合"))
						c.value = t.value;
					if (c.selectedIndex < 0)
						c.selectedIndex = 0;
				}
			}
				
			//console.log(t.name+"="+document.activeElement.name+"-"+textSelect);
			if (t.name == document.activeElement.name && textSelect != null) {
				doTextSelect_onblur = true;
				
				textSelect.onblur = textSelect_onblur;
				textSelect_onblur = null;
				var textSelectTemp = textSelect;
				textSelect = null;
				if (textSelectTemp.onblur)
					textSelectTemp.onblur();
			}
		}
		
		if (!doTextSelect_onblur) {
			//console.log(doTextSelect_onblur);
		
			if (textSelect_list != null) {
				if (c[0]!=null && c[0].name!=null) {
					for (var iTemp = 0; iTemp<c.length; iTemp++) {
						if (c[iTemp].options[0].text.endsWith("筆符合")) {
							//if (c[iTemp].selectedIndex == 0)//若是查詢關鍵字，則不檢查錯誤。
								//t.value = "";
							c[iTemp].options.length = 0;
							for (var j = 0; j<textSelect_list[iTemp].length; j++)
								c[iTemp].add(textSelect_list[iTemp][j], j);
						}
						c[iTemp].selectedIndex = 0;
					}
				}
				else {
					//alert(c.options.length+":"+textSelect_list[0].options.length);
					if (c.options[0].text.endsWith("筆符合")) {
						//if (c.selectedIndex == 0)//若是查詢關鍵字，則不檢查錯誤。
							//t.value = "";
						c.options.length = 0;
						for (var j = 0; j<textSelect_list[0].length; j++)
							c.add(textSelect_list[0][j], j);
					}
					c.selectedIndex = 0;
				}
				textSelect_list = null;
			}
		
			if (textSelect_onblur != null) {
				textSelect.onblur = textSelect_onblur;
				textSelect_onblur = null;
				var textSelectTemp = textSelect;
				textSelect = null;
				textSelectTemp.onblur();
			}
			
			if ((b || findObj("r").value != "F") && t.value != "" && (i != null || d != null) && check) {
			
				var r = checkNum(t.value, i, d);
		
				if (isNaN(r)) {
				
					if (r.length > 1) {
						if (r.charAt(0) == "I") {
							setCheckNoPass(n+' 整數超過 '+i+'位!', t);
						}
						else if (r.charAt(0) == "D") {
							setCheckNoPass(n+' 小數超過 '+d+'位!', t);
						}
					}
					else {
						if (r == ",") {
							var sOld = t.value;
							var sNew = "";
							for (var iTemp=0;iTemp<sOld.length;iTemp++) {
								cIndex = sOld.charAt(iTemp);							
								if (cIndex != ",")
									sNew += cIndex;
							}
							t.value = addCommas(sNew);
						}
						else {
							setCheckNoPass(n+' 數值格式錯誤 ['+r+'] !', t);
						}
					}
					return;	
				}
			}
				
			var tmp = "";
			var st = t.value;			
			if (findObj("r").value == "F") {
				if (t.value.indexOf(">=") == 0 || t.value.indexOf("<=") == 0) {
					tmp = st.substring(0, 2);
					st = t.value.substring(2);
				}
				else if (t.value.indexOf("~") == 0 || t.value.indexOf("?") == 0 || t.value.indexOf("=") == 0 || t.value.indexOf(">") == 0 || t.value.indexOf("<") == 0) {
					tmp = st.substring(0, 1);
					st = t.value.substring(1);
				}
				
				var lastp = st.lastIndexOf(",");
				if (lastp != -1) {
					tmp = tmp + st.substring(0, lastp+1);
					st = st.substring(lastp+1);
				}
			}			
							
			if (c != null) {		
				if (c[0]!=null && c[0].name!=null) {
					for (var iTemp = 0; iTemp<c.length; iTemp++) {
						if(st=="") {
							for (var j = 0; j<c.length; j++) {
								c[j].selectedIndex = 0;
							}
						}
						else {
							var cSelect = c[iTemp];
							if (cSelect[cSelect.selectedIndex].value != st) {
								for (var cIndex = 0; cIndex<cSelect.options.length; cIndex++) {
									if (cSelect.options[cIndex].value == st) {
										cSelect.selectedIndex = cIndex;
										break;
									}
								}
							}
						}
							
						if (c[iTemp].value != st) {
							if ((b || findObj("r").value != "F") && check) {
								if (st != "") {
									setCheckNoPass(n+' 不存在!', t, null, c);
									return;
								}
								else {
									t.value = c[iTemp].value;
								}
							}	
						}
					}
				}
				else {
					
					if(st=="") {
						c.selectedIndex = 0;
					}	
					else {
						if (c[c.selectedIndex].value != st) {
							for (var cIndex = 0; cIndex<c.options.length; cIndex++) {
								if (c.options[cIndex].value == st) {
									c.selectedIndex = cIndex;
									break;
								}
							}
						}
					}
					
					if (c.value != st) {
						if ((b || findObj("r").value != "F") && check) {
							if (st != "") {
								setCheckNoPass(n+' 不存在!', t, c, null);
								return;
							}
							else {
								t.value = c.value;
							}
						}
					}
				}
			}	
		}
		
		if (!check_doChange(t, changecheck))
			return;
		
		doBlur = null;
	
		return true;
		
	}
	
	function toAddZero(obj, len) {
		while(obj.value.length!=0 && obj.value.length<len && !isNaN(obj.value)){
			obj.value='0'+obj.value;
		}
	}
	
	function setCheckNoPass(msg, t, c, ca) {
		alert(msg);
		doMouseDown = false;
		if (!appNetscape) {
			setTextRange(t);
			doChange = null;
		}
		else {
			t.value = "";
			if (c != null) {
				c.selectedIndex = 0;
				t.value = c.value;
			}
			if (ca != null) {
				for (var j = 0; j<ca.length; j++) {
					ca[j].selectedIndex = 0;
				}
				t.value = ca[0].value;
			}
			t.focus();
		}
	}
	
	function choice_onchange(c) {
		//console.log(c.selectedIndex+":"+c.value+"-"+c.options[c.selectedIndex].text+","+textSelect_filter);
		var ct = findObj(c.name);

		if (ct[0]!=null && ct[0].name!=null) {
			for (var iTemp = 0; iTemp<ct.length; iTemp++) {
				ct[iTemp].value = c.value;
			}
		}
		
		var t = findObj(c.name.substring(0, c.name.length-5));		
	
		var tmp = t.value;
		if (tmp.indexOf("~")==0) {
			i = tmp.indexOf(",");
			if (i != -1) {
				if (i == tmp.length-1)
					t.value = tmp + c.value;
				else
					t.value = tmp.substring(0,i+1) + c.value;
			}
			else {
				t.value = tmp.substring(0,1) + c.value;
			}
		}
		else if (tmp.indexOf("?")==0) {
			i = tmp.lastIndexOf(",");
			if (i != -1) {
				if (i == tmp.length-1)
					t.value = tmp + c.value;
				else
					t.value = tmp.substring(0,i+1) + c.value;
			}
			else {
				t.value = tmp.substring(0,1) + c.value;
			}
		}
		else if (tmp.indexOf(">=")==0 || tmp.indexOf("<=")==0) {
			t.value = tmp.substring(0,2) + c.value;
		}
		else if (tmp.indexOf("=")==0 || tmp.indexOf(">")==0 || tmp.indexOf("<")==0) {
			t.value = tmp.substring(0,1) + c.value;
		}
		else if (c.value == "" && c.options[c.selectedIndex].text.endsWith("筆符合")) {
			t.value = textSelect_filter;
		}		
		else {
			t.value = c.value;
		}
		
		/*
		t.focus();
	
		try {
			if (appNetscape) {
				t.setSelectionRange(0, 0); 
			}
			else {
				var range = t.createTextRange();
				if (range) {
					range.moveStart('character', t.value.length);						
					range.moveEnd('character', t.value.length);
					range.select();
				}
			}
		}
		catch (e) {
			//alert(e.name + ":" + e.message);
		}
		*/
	
		return true;
	}	
	
	function checkbox_onclick(c,v1,v2){
		if (c.checked) {
			findObj(c.name.substring(0, c.name.length-5)).value = v1;
		}
		else {
			findObj(c.name.substring(0, c.name.length-5)).value = v2;
		}	
		
		return true;	
	}

/*	exp:YYYYNNYN
	function checklist_onclick(c,v1,v2,sep,i){
		var o = findObj(c.name.substring(0, c.name.length-5));
		if (sep != "") {
			s = o.value.split(sep);
			if (s.length < i) {
				for (var j=s.length;j<i;j++) {
					s[j] = "";
				}
			}
			if (c.checked) {
				s[i - 1] = v1;
			}
			else {
				s[i - 1] = v2;
			}
			o.value = s[0];
			for (var j=1;j<s.length;j++) {
				o.value += sep+s[j];
			}
		}
		else {
			if (o.value.length < i) {
				var l = o.value.length;
				for (var j=l;j<i;j++) {
					o.value += " ";
				}
			}
			if (c.checked) {
				o.value = o.value.substring(0, i - 1)+v1+o.value.substring(i, o.value.length);
			}
			else {
				o.value = o.value.substring(0, i - 1)+v2+o.value.substring(i, o.value.length);
			}
		}
		//alert('.'+o.value+'.');
		return true;	
	}
*/
	
	function findColumn(n, row, frame) {
		
		if (row == null)
			row = "1";
		
		if (frame == null) {
			var cn = vbColumn;
			for (var ic = 0; ic < cn.length; ic++) {
				if (cn[ic] == n) {
					return findObj(vbID+"R"+row+"C"+(ic+1));
				}
			}
		}
		else {
			var cn = frame.vbColumn;
			for (var ic = 0; ic < cn.length; ic++) {
				if (cn[ic] == n) {
					return findObj(frame.vbID+"R"+row+"C"+(ic+1), frame);
				}
			}
		}		
		
		return null;
	}
			
	function findIFrame(n, frame) {//not use has bug!!
		//fix Chrome with Name (use ID is better, not to do this.)
		var iframesByName = null;
		if (frame)
			iframesByName = frame.document.getElementsByTagName('iframe');
		else
			iframesByName = document.getElementsByTagName('iframe');
				
		var objif = null;
		if (iframesByName) {
			for (var i=0; i<iframesByName.length; i++) {
				if (iframesByName[i].name == n) {
					objif = iframesByName[i];
					break;
				}
			}
		}
		return objif;
	}
		
	function findObj(n, frame) {
		try {			
			if (n.indexOf("styleSheets.") == 0) {
				var wdss = document.styleSheets;
				if (frame)
					wdss = frame.document.styleSheets;			
				var ssn = n.substring(12);				
				for (var ss=0; ss<wdss.length; ss++) {
					var cr;
					if (appNetscape)
						cr = wdss[ss].cssRules;
					else	
						cr = wdss[ss].rules;
					
					for (var rs=0; rs<cr.length; rs++) {
						var csText = cr[rs].selectorText;
						if (csText == null)
							csText = cr[rs].toString();
						if (csText.toUpperCase().indexOf("FONTFACE") != -1) {
							if (ssn.toUpperCase().endsWith("FONT-FACE"))
								return cr[rs];
						}
						else {
							if (csText.toUpperCase() == ssn.toUpperCase())				
								return cr[rs];
							if (csText.toUpperCase() == "."+ssn.toUpperCase())				
								return cr[rs];
						}
					}
				}
				return null;
			}
			
			if (n==null)
				return;
			var x = null;
			var f = n.indexOf(".");
			var i = n.indexOf("[");
			var j = n.indexOf("]");
			
			if(i != -1) {
				if (f == -1) {
					var o = document.all[n.substring(0,i)];
					if (frame)
						o = frame.document.all[n.substring(0,i)];
					
					if (o != null)
						x = o[parseInt(n.substring(i+1,j))];
				}
				else {
					var frameEnd = false;
					var ft = n.indexOf(".");
					var o = null;
					while (ft != -1 && !frameEnd) {
						var ot = null;
						if (frame)
							ot = eval(frame.name+"."+n.substring(0,ft));
						else
							ot = eval("self."+n.substring(0,ft));
						
						if (ot != null) {
							f = ft;
							o = ot;
						}
						else
							frameEnd = true;
						ft = n.indexOf(".",ft+1);
					}
					
					if (o != null) {
						o = o.document.all[n.substring(f+1,i)];
						if (o == null && n.substring(f+1,i).indexOf(".") == -1) {
							o = eval(n.substring(0,i));
						}
					}
					else {
						o = document.all[n.substring(0,i)];
						if (frame)
							o = frame.document.all[n.substring(0,i)];
					}
						
					if (o != null)
						x = o[parseInt(n.substring(i+1,j))];
				}	
			}
			else {
				if (f == -1) {
					x = document.all[n];
					if (frame)
						x = frame.document.all[n];
				}
				else {
					var frameEnd = false;
					var ft = n.indexOf(".");
					var o = null;
					while (ft != -1 && !frameEnd) {
						var ot = null;
						if (frame) {
							ot = eval(frame.name+"."+n.substring(0,ft));
						}
						else {
							ot = eval("self."+n.substring(0,ft));
						}
							
						if (ot != null) {
							f = ft;
							o = ot;
						}
						else
							frameEnd = true;
						ft = n.indexOf(".",ft+1);
					}
					
					if (o != null) {
						var nsub = n.substring(f+1);
						if (nsub != "") {
							x = o.document.all[nsub];
							if (x == null && nsub.indexOf(".") == -1) {
								x = eval(n);
							}
						}
					}
					else {
						x = document.all[n];	
						if (frame)
							x = frame.document.all[n];	
					}
				}	
			}
			
			return x;
		} 
		catch (e) {
			//alert(e.name + ":" + e.message);
		}
	}
	
	function findArrayIndex(o) {
		var i = 0;
	
		if (o.name != null) {
			for(i=0; i<findObj(o.name).length; i++) {
				if (o == findObj(o.name)[i])
					return i;
			}	
		}
		else {
			for(i=0; i<findObj(o[0].name).length; i++) {
				if (o == findObj(o[0].name)[i])
					return i;
			}
		}
		
		return -1;
	}

	function replaceAll (str, orig, chan) {
		var arrayStr = str.split(orig);
		var newStr = str;
		for (var i=0; i < arrayStr.length; i++) {
			if (i==0)
				newStr = arrayStr[i];
			else	
				newStr += chan + arrayStr[i];
		}
		return newStr;
	}

	function setTextRangeFocus(t) {			
		if (appNetscape && t.type.length >= 6 && t.type.toUpperCase().substring(0, 6) == "SELECT") {
			setTimeout("refocus('"+t.name+"')", 0);
		}
		else {
			t.focus();
		}
	}
	function setTextRange(f, t) {
		
		try {
			if (f != null && f.type != null && (f.type.toUpperCase() == "PASSWORD" || f.type.toUpperCase() == "TEXT" || f.type.toUpperCase() == "TEL" || f.type.toUpperCase() == "TEXTAREA")) {
				if (appNetscape) {
					if (t != null)
						f.setSelectionRange(0, 0);
					else {
						if (setTextRange.arguments.length == 1)
							setTextRangeFocus(f);
						f.setSelectionRange(0, f.value.length);
					}
				}
				else {
					var range = f.createTextRange();
					if (range) {
						if (t != null)
							range.moveStart('character', f.value.length);
						else {
							if (setTextRange.arguments.length == 1)
								setTextRangeFocus(f);
							range.moveStart('character', 0);
						}						
						range.moveEnd('character', f.value.length);						
						range.select();
					}
				}
			}
		} 
		catch (e) {
			//alert(e.name + ":" + e.message);
		}
		
		if (setTextRange.arguments.length == 3) {	
			
			try {
				if (t != null && t.type != null && (t.type.toUpperCase() == "PASSWORD" || t.type.toUpperCase() == "TEXT" || t.type.toUpperCase() == "TEL" || t.type.toUpperCase() == "TEXTAREA")) {
					if (appNetscape) {
						//有時候當有Norton瀏覽器防護:MSIE CreateTextRange Remote Code Execution，第一次進畫面無法run[setSelectionRange]。
						t.setSelectionRange(0, t.value.length);
					}
					else {
						var range = t.createTextRange();
						if (range) {
							if (t.type.toUpperCase() == "TEXTAREA")
								range.moveStart('character', t.value.length);
							else	
								range.moveStart('character', 0);			
							range.moveEnd('character', t.value.length);						
							range.select();
						}
					}
				}
			} 
			catch (e) {
				//alert(e.name + ":" + e.message);
			}
			
			setTextRangeFocus(t);
		}
		
	}
	
	//Firefox onkeydown在<select>切換focus時，有異常問題，會進不去focus
	function refocus(objName){
		findObj(objName).focus();
	}
						
	function d(c, r, u, dn) {
		
		var event = (window.event?window.event:d.arguments[d.arguments.length-1]);
		var srcObj;
		if(event)
			srcObj = (event.srcElement?event.srcElement:event.target);
		//event.cancelBubble=true;
		//alert("d:"+doKeyDown);

		var o;
		
		if (c == null || isNaN(c)) {
			var op = findObj(c);
			var on = findObj(r);
			var ou = findObj(u);
			var od = findObj(dn);
			
			var ai = findArrayIndex(srcObj);
			if (ai != -1) {
				var sObj = findObj(srcObj.name);
				if (ai != 0) {
					op = sObj[ai-1];
				}
				else {
					if (op != null && op[0] != null && op[0].name != null) {
						op = op[op.length-1];
					}
				}
				
				if (ai != sObj.length-1) {
					on = sObj[ai+1];
				}
				else {
					if (on != null && on[0] != null && on[0].name != null) {
						on = on[0];
					}
				}
			}
			else {
				if (op != null && op[0] != null && op[0].name != null) {
					op = op[op.length-1];
				}
				
				if (on != null && on[0] != null && on[0].name != null) {
					on = on[0];
				}
			}
			
			if (ou != null && ou[0] != null && ou[0].name != null) {
				ou = ou[0];
			}
			
			if (od != null && od[0] != null && od[0].name != null) {
				od = od[0];
			}					

			if (op != null && op.type != null && op.disabled != true && op.readOnly != true && op.type.toUpperCase() != "HIDDEN" && op.type.toUpperCase() != "BUTTON" && op.type.toUpperCase() != "SUBMIT"){
				if (doKeyDown && event.keyCode == 13 && event.shiftKey == true && (srcObj == null || srcObj.type == null || srcObj.type.toUpperCase() != "TEXTAREA")) {
					doKeyDown = false;
					setTextRange(srcObj, op, event);
					return;
				}
				else if (doKeyDown && event.ctrlKey == true) {
					if (event.keyCode == 37) {
						doKeyDown = false;
						setTextRange(srcObj, op, event);
						return;
					}
				}	
			}
			
			if (on != null && on.type != null && on.disabled != true && on.readOnly != true && on.type.toUpperCase() != "HIDDEN" && on.type.toUpperCase() != "BUTTON" && on.type.toUpperCase() != "SUBMIT"){
				if (doKeyDown && event.keyCode == 13 && event.shiftKey == false && (srcObj == null || srcObj.type == null || srcObj.type.toUpperCase() != "TEXTAREA")) {
					doKeyDown = false;
					setTextRange(srcObj, on, event);
					return;
				}
				else if (doKeyDown && event.ctrlKey == true) {
					if (event.keyCode == 39) {
						doKeyDown = false;
						setTextRange(srcObj, on, event);
						return;
					}
				}
			}
			
			if (ou != null && ou.type != null && ou.disabled != true && ou.readOnly != true && ou.type.toUpperCase() != "HIDDEN" && ou.type.toUpperCase() != "BUTTON" && ou.type.toUpperCase() != "SUBMIT"){
				if (doKeyDown && event.ctrlKey == true) {
					if (event.keyCode == 38) {
						doKeyDown = false;
						setTextRange(srcObj, ou, event);
						return;
					}
				}	
			}
			
			if (od != null && od.type != null && od.disabled != true && od.readOnly != true && od.type.toUpperCase() != "HIDDEN" && od.type.toUpperCase() != "BUTTON" && od.type.toUpperCase() != "SUBMIT"){
				if (doKeyDown && event.ctrlKey == true) {
					if (event.keyCode == 40) {
						doKeyDown = false;
						setTextRange(srcObj, od, event);
						return;
					}
				}	
			}
			
			if (doKeyDown) {
				if (event.keyCode == 13) {
					doKeyDown = false;
					setTextRange(srcObj, event);
					return;
				}
				else if (event.ctrlKey == true) {
					if (event.keyCode == 37 || event.keyCode == 39 || event.keyCode == 38 || event.keyCode == 40) {
						doKeyDown = false;
						setTextRange(srcObj, event);
						return;
					}
				}
			}
			
			return;
			
		}
			
		var ar = new Array;

		if (doKeyDown && event.keyCode == 13 && event.shiftKey == true && (srcObj == null || srcObj.type == null || srcObj.type.toUpperCase() != "TEXTAREA")) {
			ar = kp(c, r, o, u);
			c = ar[0];
			o = ar[1];
					
			while (o == null || o.type == null || o.disabled || o.readOnly || o.type.toUpperCase() == "HIDDEN" || o.type.toUpperCase() == "BUTTON" || o.type.toUpperCase() == "SUBMIT") {
				if (c <= 0) {
					c = vbColumnCount+1;
					r--;
					if (r < 1) {
						doKeyDown = false;
						setTextRange(srcObj, event);
						return;
					}
				}
				
				ar = kp(c, r, o, u);
				c = ar[0];
				o = ar[1];	
							
			}
			doKeyDown = false;
			setTextRange(srcObj, o, event);
		}
		else if (u == true || (doKeyDown && event.keyCode == 13 && event.shiftKey == false && (srcObj == null || srcObj.type == null || srcObj.type.toUpperCase() != "TEXTAREA"))) {		

			var sn = false;
			if (u == true) {
				u = null;
				sn = true;
			}
			//vbRealSize is jsp row count
			if (sn && r > vbRealSize)
				r = vbRealSize;
				
			ar = kn(c, r, o, u);
			c = ar[0];
			o = ar[1];

			while (o == null || o.type == null || o.disabled || o.readOnly || o.type.toUpperCase() == "HIDDEN" || o.type.toUpperCase() == "BUTTON" || o.type.toUpperCase() == "SUBMIT") {

				if (c > vbColumnCount) {
					c = -1;
					r++;

					if (r > vbRealSize) {
						try { eval('isRowNew'); } catch (e) { isRowNew = false; }
						
						if (isRowNew != null && isRowNew && findObj("r").value != "F" && findObj("r").value != "") {
							findObj("c").value = r;
							if (!sn)
								doKeyDown = false;
							setTextRange(srcObj, event);
							if (!sn) {
								if(srcObj.onblur != null)
									srcObj.onblur();
								pageSubmit('TabNew');
							}
							return;
						}
						else {
							if (!sn)
								doKeyDown = false;
							setTextRange(srcObj, event);
							return;
						}	
					}	
				}
			
				ar = kn(c, r, o, u);
				c = ar[0];
				o = ar[1];	
						
			}
			if (!sn) {
				doKeyDown = false;
			}
			setTextRange(srcObj, o, event);
		}
		else if (doKeyDown && event.ctrlKey == true) {
			if (event.keyCode == 37 ) {	

				ar = kp(c, r, o, u);
				c = ar[0];
				o = ar[1];
						
				while (o == null || o.type == null || o.disabled || o.readOnly || o.type.toUpperCase() == "HIDDEN" || o.type.toUpperCase() == "BUTTON" || o.type.toUpperCase() == "SUBMIT") {
					if (c <= 0) {
						c = vbColumnCount+1;
						r--;
						if (r < 1) {
							doKeyDown = false;
							setTextRange(srcObj, event);
							return;
						}
					}
					
					ar = kp(c, r, o, u);
					c = ar[0];
					o = ar[1];	
								
				}
				doKeyDown = false;
				setTextRange(srcObj, o, event);
			}
			else if (event.keyCode == 38 ) {
				
				var ai = -1;
				var checkObj;		

				if (u != null && u.type != null && u.disabled != true && u.readOnly != true && u.type.toUpperCase() == "RADIO") {
					ai = findArrayIndex(u);
				}
				
				if (u.name.indexOf("check") == u.name.length-5) {
					checkObj = findObj(u.name);
					if (checkObj[0]!=null && checkObj[0].name!=null) {
						ai = findArrayIndex(u);
					}
				}				
			
				r--;
				if (c==0) {
					o = findObj("checkDelete"+vbID+"R"+r+"C"+c);
				}
				else {
					o = findObj(vbID+"R"+r+"C"+c);
					if (o != null && checkObj == null) {
						if (ai != -1) {
							if (o[0] != null && o[0].type != null && o[0].disabled != true && o[0].readOnly != true && o[0].type.toUpperCase() == "RADIO") {
								o = findObj(o[0].name)[ai];	
							}							
						}
					}
					else if (o != null) {
						if (ai == -1) {
							o = findObj(o.name+'check');
						}
						else {
							o = findObj(o.name+'check')[ai];
						}
					}					
				}	
					
				while (o == null || o.type == null || o.disabled || o.readOnly || o.type.toUpperCase() == "HIDDEN" || o.type.toUpperCase() == "BUTTON" || o.type.toUpperCase() == "SUBMIT") {
					if (r < 1) {
						doKeyDown = false;
						setTextRange(srcObj, event);
						return;
					}
					
					r--;
					if (c==0) {
						o = findObj("checkDelete"+vbID+"R"+r+"C"+c);
					}
					else {
						o = findObj(vbID+"R"+r+"C"+c);
						if (o != null && checkObj == null) {
							if (ai != -1) {
								if (o[0] != null && o[0].type != null && o[0].disabled != true && o[0].readOnly != true && o[0].type.toUpperCase() == "RADIO") {
									o = findObj(o[0].name)[ai];	
								}							
							}
						}
						else if (o != null) {
							if (ai == -1) {
								o = findObj(o.name+'check');
							}
							else {
								o = findObj(o.name+'check')[ai];
							}
						}					
					}				
				}
				doKeyDown = false;
				setTextRange(srcObj, o, event);
			}
			else if (event.keyCode == 39 ) {
		
				ar = kn(c, r, o, u);
				c = ar[0];
				o = ar[1];
					
				while (o == null || o.type == null || o.disabled || o.readOnly || o.type.toUpperCase() == "HIDDEN" || o.type.toUpperCase() == "BUTTON" || o.type.toUpperCase() == "SUBMIT") {
					if (c > vbColumnCount) {
						c = -1;
						r++;
						if (r > vbRealSize) {
							doKeyDown = false;
							setTextRange(srcObj, event);
							return;
						}
					}
						
					ar = kn(c, r, o, u);
					c = ar[0];
					o = ar[1];
								
				}
				doKeyDown = false;
				setTextRange(srcObj, o, event);
			}
			else if (event.keyCode == 40 ) {
				
				var ai = -1;
				var checkObj;		

				if (u != null && u.type != null && u.disabled != true && u.readOnly != true && u.type.toUpperCase() == "RADIO") {
					ai = findArrayIndex(u);
				}
				
				if (u.name.indexOf("check") == u.name.length-5) {
					checkObj = findObj(u.name);
					if (checkObj[0]!=null && checkObj[0].name!=null) {
						ai = findArrayIndex(u);
					}
				}				
			
				r++;
				if (c==0) {
					o = findObj("checkDelete"+vbID+"R"+r+"C"+c);
				}
				else {
					o = findObj(vbID+"R"+r+"C"+c);
					if (o != null && checkObj == null) {
						if (ai != -1) {
							if (o[0] != null && o[0].type != null && o[0].disabled != true && o[0].readOnly != true && o[0].type.toUpperCase() == "RADIO") {
								o = findObj(o[0].name)[ai];	
							}							
						}
					}
					else if (o != null) {
						if (ai == -1) {
							o = findObj(o.name+'check');
						}
						else {
							o = findObj(o.name+'check')[ai];
						}
					}					
				}
				
				while (o == null || o.type == null || o.disabled || o.readOnly || o.type.toUpperCase() == "HIDDEN" || o.type.toUpperCase() == "BUTTON" || o.type.toUpperCase() == "SUBMIT") {
					if (r > vbRealSize) {
						doKeyDown = false;
						setTextRange(srcObj, event);
						return;	
					}
					
					r++;
					if (c==0) {
						o = findObj("checkDelete"+vbID+"R"+r+"C"+c);
					}
					else {
						o = findObj(vbID+"R"+r+"C"+c);
						if (o != null && checkObj == null) {
							if (ai != -1) {
								if (o[0] != null && o[0].type != null && o[0].disabled != true && o[0].readOnly != true && o[0].type.toUpperCase() == "RADIO") {
									o = findObj(o[0].name)[ai];	
								}							
							}
						}
						else if (o != null) {
							if (ai == -1) {
								o = findObj(o.name+'check');
							}
							else {
								o = findObj(o.name+'check')[ai];
							}
						}					
					}				
				}
				doKeyDown = false;
				setTextRange(srcObj, o, event);
			}
		}
		return true;
	}	
	
		
	function kn(c, r, o, u){
		var ar = new Array;
		var arf = new Array;
		var checkObj;			
		if (u == null) {
			arf = knf(r, c);
			ar[0] = arf[0];
			ar[1] = arf[1];
			return ar;
		}
		else if (u != null && u.type != null && (u.type.toUpperCase() == "RADIO" || u.type.toUpperCase() == "CHECKBOX")) {
			var ai = findArrayIndex(u);
			ar[0] = c;
			if (ai == -1)
				ar[1] = findObj(u.name)[0];
			else
				ar[1] = findObj(u.name)[ai+1];
			if (ar[1] == null) {
				arf = knf(r, c);
				ar[0] = arf[0];
				ar[1] = arf[1];
			}
			return ar;
		}
		else if (u[0] != null && u[0].type != null && (u[0].type.toUpperCase() == "RADIO" || u[0].type.toUpperCase() == "CHECKBOX")) {
			var ai = findArrayIndex(u);
			ar[0] = c;
			if (ai == -1)
				ar[1] = findObj(u[0].name)[0];
			else
				ar[1] = findObj(u[0].name)[ai+1];
			if (ar[1] == null) {
				arf = knf(r, c);
				ar[0] = arf[0];
				ar[1] = arf[1];
			}
			return ar;
		}
		
		if (u.name.indexOf("check") != u.name.length-5) {
			checkObj = findObj(u.name+'check');
		}
		else {
			checkObj = findObj(u.name);				
		}
				
		if (checkObj == null) {
			arf = knf(r, c);
			c = arf[0];
			o = arf[1];
		}
		else {
			if (checkObj[0]==null || checkObj[0].name==null) {
				if (u.name.indexOf("check") != u.name.length-5) {
					o = checkObj;
				}
				else {
					arf = knf(r, c);
					c = arf[0];
					o = arf[1];
				}
			}
			else {
				if (u.name.indexOf("check") != u.name.length-5) {
					o = checkObj[0];
				}
				else {
					var ai = findArrayIndex(u);
					if (ai == checkObj.length-1) {
						arf = knf(r, c);
						c = arf[0];
						o = arf[1];
					}
					else {
						o = checkObj[ai+1];
					}
					
				}
			}
		}

		ar[0] = c;
		ar[1] = o;	
		return ar;
	}
			
	function kp(c, r, o, u){	
		var ar = new Array;
		var arf = new Array;
		var checkObj;
		if (u == null) {
			arf = knp(r, c);
			ar[0] = arf[0];
			ar[1] = arf[1];
			return ar;
		}
		else if (u != null && u.type != null && (u.type.toUpperCase() == "RADIO" || u.type.toUpperCase() == "CHECKBOX")) {
			var ai = findArrayIndex(u);
			ar[0] = c;
			if (ai == -1)
				ar[1] = findObj(u.name)[findObj(u.name).length-1];
			else if (ai > 0)
				ar[1] = findObj(u.name)[ai-1];
			if (ar[1] == null) {
				arf = knp(r, c);
				ar[0] = arf[0];
				ar[1] = arf[1];
			}
			return ar;
		}
		else if (u[0] != null && u[0].type != null && (u[0].type.toUpperCase() == "RADIO" || u[0].type.toUpperCase() == "CHECKBOX")) {
			var ai = findArrayIndex(u);
			ar[0] = c;
			if (ai == -1)
				ar[1] = findObj(u[0].name)[findObj(u[0].name).length-1];
			else if (ai > 0)
				ar[1] = findObj(u[0].name)[ai-1];	
			if (ar[1] == null) {
				arf = knp(r, c);
				ar[0] = arf[0];
				ar[1] = arf[1];
			}
			return ar;			
		}
				
		if (u.name.indexOf("check") != u.name.length-5) {
			arf = knp(r, c);
			c = arf[0];
			o = arf[1];
			if (o != null)
				checkObj = findObj(o.name+'check');
		}
		else {
			checkObj = findObj(u.name);				
		}
					
		if (checkObj != null) {
			if (checkObj[0]==null || checkObj[0].name==null) {
				if (u.name.indexOf("check") == u.name.length-5) {					
					o = findObj(u.name.substring(0, u.name.length-5));
				}
				else {
					arf = knp(r, c);
					c = arf[0];
					o = arf[1];
				}
			}
			else {
				if (u.name.indexOf("check") == u.name.length-5) {
					o = checkObj[checkObj.length-1];
				}
				else {
					var ai = findArrayIndex(u);
					if (ai == 0) {
						arf = knp(r, c);
						c = arf[0];
						o = arf[1];
					}
					else {
						o = checkObj[ai-1];
					}	
				}
			}
		}

		ar[0] = c;
		ar[1] = o;
		return ar;
	}
	
	function knf(r, c) {
		var arf = new Array;
		var o;
		c++;
		if (c==0) {
			o = findObj("checkDelete"+vbID+"R"+r+"C"+c);
		}
		else {
			o = findObj(vbID+"R"+r+"C"+c);
			if (o != null && o.type != null && (o.type.toUpperCase() == "RADIO" || o.type.toUpperCase() == "CHECKBOX")) {
				o = findObj(o.name)[0];
			}
			else if (o != null && o[0] != null && o[0].type != null && (o[0].type.toUpperCase() == "RADIO" || o[0].type.toUpperCase() == "CHECKBOX")) {
				o = findObj(o[0].name)[0];
			}
			else if (o != null && o.type != null && o.type.toUpperCase() == "HIDDEN") {
				o = findObj(vbID+"R"+r+"C"+c+'check');
			}
		}	
		
		arf[0] = c;
		arf[1] = o;
		return arf;
	}
	
	function knp(r, c) {
		var arf = new Array;
		var o;				
		c--;
		if (c==0) {
			o = findObj("checkDelete"+vbID+"R"+r+"C"+c);
		}
		else {
			o = findObj(vbID+"R"+r+"C"+c);
			if (o != null) {
				if (o.type != null && (o.type.toUpperCase() == "RADIO" || o.type.toUpperCase() == "CHECKBOX")) {
					o = findObj(o.name)[findObj(o.name).length-1];
				}
				else if (o[0] != null && o[0].type != null && (o[0].type.toUpperCase() == "RADIO" || o[0].type.toUpperCase() == "CHECKBOX")) {
					o = findObj(o[0].name)[findObj(o[0].name).length-1];
				}
				else {
					checkObj = findObj(o.name+'check');
					if (checkObj != null) {
						if (checkObj[0]==null || checkObj[0].name==null) {
							o = checkObj;
						}
						else {
							o = checkObj[checkObj.length-1];
						}
					}
				}	
			}
		}		
		
		arf[0] = c;
		arf[1] = o;
		return arf;
	}
	
	function list_onchange(l) {
		var ltv = findObj(l.name+"value");
		var lti = findObj(l.name+"item");
		if (ltv != null && lti != null) {
			ltv.value = "";
			lti.value = "";
			for (var iTemp1 = 0; iTemp1<l.length; iTemp1++) {
				ltv.value += "#S@" + l[iTemp1].value + "#E@";
				lti.value += "#S@" + l[iTemp1].text + "#E@";				
			}
			return true;
		}
	}
	function list_move_to(listFrom, listTo, allSelect, withDelete){
		if (allSelect == null)
			allSelect = false;
		if (withDelete == null)
			withDelete = true;
			
		var oFrom = findObj(listFrom);
		var oTo = findObj(listTo);
		var ot = null;
		if (oTo != null)
			ot = findObj(listTo+"value");
		
		for (var iTemp2 = 0; ot!=null && iTemp2<oTo.length; iTemp2++) {
			oTo[iTemp2].selected = false;
		}
				
		for (var iTemp1 = 0; iTemp1<oFrom.length; iTemp1++) {
			if(allSelect || oFrom[iTemp1].selected) {
				var b = true;
				for (var iTemp2 = 0; ot!=null && iTemp2<oTo.length; iTemp2++) {
					if(oFrom[iTemp1].text == oTo[iTemp2].text || oFrom[iTemp1].value == oTo[iTemp2].value) {
						b = false;
						oTo[iTemp2].selected = true;
						break;
					}					
				}
				if (ot != null && b) {
					oTo[oTo.length] = new Option(oFrom[iTemp1].text, oFrom[iTemp1].value, false, true);
				}
				
				if (withDelete) {
					oFrom[iTemp1] = null;
					iTemp1--;
				}
			}
		}
		if (withDelete)
			list_onchange(oFrom);
		if (ot != null)
			list_onchange(oTo);
		return true;
	}
	
	function list_move_up(columnlist) {
		var list = findObj(columnlist);
		for (var iTemp1 = 0; iTemp1<list.length; iTemp1++) {
			if(list[iTemp1].selected) {
				if (iTemp1-1 != -1 && list[iTemp1-1].selected == false) {
					var temptext = list[iTemp1].text;
					var tempvalue = list[iTemp1].value;
					
					list[iTemp1].text = list[iTemp1-1].text;
					list[iTemp1].value = list[iTemp1-1].value;
					list[iTemp1-1].text = temptext;
					list[iTemp1-1].value = tempvalue;
					list[iTemp1].selected = false;
					list[iTemp1-1].selected = true;
				}
			}
		}
		list_onchange(list);
	}	
	function list_move_down(columnlist) {
		var list = findObj(columnlist);
		for (var iTemp1 = list.length-1; iTemp1>=0; iTemp1--) {
			if(list[iTemp1].selected) {
				if (iTemp1+1 != list.length && list[iTemp1+1].selected == false) {
					var temptext = list[iTemp1].text;
					var tempvalue = list[iTemp1].value;
					
					list[iTemp1].text = list[iTemp1+1].text;
					list[iTemp1].value = list[iTemp1+1].value;
					list[iTemp1+1].text = temptext;
					list[iTemp1+1].value = tempvalue;
					list[iTemp1].selected = false;
					list[iTemp1+1].selected = true;
				}
			}
		}
		list_onchange(list);
	}
	
	
	function checkColor(color_string) {
		
		if (color_string == "")
			return false;
			
	    var b = false;
	
	    color_string = color_string.toLowerCase();
	
	    var simple_colors = {
	        aliceblue: 'f0f8ff',
	        antiquewhite: 'faebd7',
	        aqua: '00ffff',
	        aquamarine: '7fffd4',
	        azure: 'f0ffff',
	        beige: 'f5f5dc',
	        bisque: 'ffe4c4',
	        black: '000000',
	        blanchedalmond: 'ffebcd',
	        blue: '0000ff',
	        blueviolet: '8a2be2',
	        brown: 'a52a2a',
	        burlywood: 'deb887',
	        cadetblue: '5f9ea0',
	        chartreuse: '7fff00',
	        chocolate: 'd2691e',
	        coral: 'ff7f50',
	        cornflowerblue: '6495ed',
	        cornsilk: 'fff8dc',
	        crimson: 'dc143c',
	        cyan: '00ffff',
	        darkblue: '00008b',
	        darkcyan: '008b8b',
	        darkgoldenrod: 'b8860b',
	        darkgray: 'a9a9a9',
	        darkgreen: '006400',
	        darkkhaki: 'bdb76b',
	        darkmagenta: '8b008b',
	        darkolivegreen: '556b2f',
	        darkorange: 'ff8c00',
	        darkorchid: '9932cc',
	        darkred: '8b0000',
	        darksalmon: 'e9967a',
	        darkseagreen: '8fbc8f',
	        darkslateblue: '483d8b',
	        darkslategray: '2f4f4f',
	        darkturquoise: '00ced1',
	        darkviolet: '9400d3',
	        deeppink: 'ff1493',
	        deepskyblue: '00bfff',
	        dimgray: '696969',
	        dodgerblue: '1e90ff',
	        feldspar: 'd19275',
	        firebrick: 'b22222',
	        floralwhite: 'fffaf0',
	        forestgreen: '228b22',
	        fuchsia: 'ff00ff',
	        gainsboro: 'dcdcdc',
	        ghostwhite: 'f8f8ff',
	        gold: 'ffd700',
	        goldenrod: 'daa520',
	        gray: '808080',
	        green: '008000',
	        greenyellow: 'adff2f',
	        honeydew: 'f0fff0',
	        hotpink: 'ff69b4',
	        indianred : 'cd5c5c',
	        indigo : '4b0082',
	        ivory: 'fffff0',
	        khaki: 'f0e68c',
	        lavender: 'e6e6fa',
	        lavenderblush: 'fff0f5',
	        lawngreen: '7cfc00',
	        lemonchiffon: 'fffacd',
	        lightblue: 'add8e6',
	        lightcoral: 'f08080',
	        lightcyan: 'e0ffff',
	        lightgoldenrodyellow: 'fafad2',
	        lightgrey: 'd3d3d3',
	        lightgreen: '90ee90',
	        lightpink: 'ffb6c1',
	        lightsalmon: 'ffa07a',
	        lightseagreen: '20b2aa',
	        lightskyblue: '87cefa',
	        lightslateblue: '8470ff',
	        lightslategray: '778899',
	        lightsteelblue: 'b0c4de',
	        lightyellow: 'ffffe0',
	        lime: '00ff00',
	        limegreen: '32cd32',
	        linen: 'faf0e6',
	        magenta: 'ff00ff',
	        maroon: '800000',
	        mediumaquamarine: '66cdaa',
	        mediumblue: '0000cd',
	        mediumorchid: 'ba55d3',
	        mediumpurple: '9370d8',
	        mediumseagreen: '3cb371',
	        mediumslateblue: '7b68ee',
	        mediumspringgreen: '00fa9a',
	        mediumturquoise: '48d1cc',
	        mediumvioletred: 'c71585',
	        midnightblue: '191970',
	        mintcream: 'f5fffa',
	        mistyrose: 'ffe4e1',
	        moccasin: 'ffe4b5',
	        navajowhite: 'ffdead',
	        navy: '000080',
	        oldlace: 'fdf5e6',
	        olive: '808000',
	        olivedrab: '6b8e23',
	        orange: 'ffa500',
	        orangered: 'ff4500',
	        orchid: 'da70d6',
	        palegoldenrod: 'eee8aa',
	        palegreen: '98fb98',
	        paleturquoise: 'afeeee',
	        palevioletred: 'd87093',
	        papayawhip: 'ffefd5',
	        peachpuff: 'ffdab9',
	        peru: 'cd853f',
	        pink: 'ffc0cb',
	        plum: 'dda0dd',
	        powderblue: 'b0e0e6',
	        purple: '800080',
	        red: 'ff0000',
	        rosybrown: 'bc8f8f',
	        royalblue: '4169e1',
	        saddlebrown: '8b4513',
	        salmon: 'fa8072',
	        sandybrown: 'f4a460',
	        seagreen: '2e8b57',
	        seashell: 'fff5ee',
	        sienna: 'a0522d',
	        silver: 'c0c0c0',
	        skyblue: '87ceeb',
	        slateblue: '6a5acd',
	        slategray: '708090',
	        snow: 'fffafa',
	        springgreen: '00ff7f',
	        steelblue: '4682b4',
	        tan: 'd2b48c',
	        teal: '008080',
	        thistle: 'd8bfd8',
	        tomato: 'ff6347',
	        turquoise: '40e0d0',
	        violet: 'ee82ee',
	        violetred: 'd02090',
	        wheat: 'f5deb3',
	        white: 'ffffff',
	        whitesmoke: 'f5f5f5',
	        yellow: 'ffff00',
	        yellowgreen: '9acd32'
	    };
	    
	    for (var key in simple_colors) {
	        if (color_string == key) {
	            color_string = simple_colors[key];
	            b = true;
	        }
	    }
	    
	    if (!b) {	
		    if (color_string.charAt(0) == '#') {
		        color_string = color_string.substring(1);
		    }
		    if (color_string.length == 6) {
	            b = true;
		    	if (b && isNaN(parseInt(color_string.charAt(0), 16)))
	            	b = false;
		    	if (b && isNaN(parseInt(color_string.charAt(1), 16)))
	            	b = false;
		    	if (b && isNaN(parseInt(color_string.charAt(2), 16)))
	            	b = false;
		    	if (b && isNaN(parseInt(color_string.charAt(3), 16)))
	            	b = false;
		    	if (b && isNaN(parseInt(color_string.charAt(4), 16)))
	            	b = false;
		    	if (b && isNaN(parseInt(color_string.charAt(5), 16)))
	            	b = false;
		    }
	    }
	    
	    return b;
	}
	
	var contentIframe = findObj("parent."+(self.name.replace(/[^\w]/g, "")));
	var apoint;
	
	function isReSizeIframe() {
		var b = false;
		if (contentIframe == null)
			return true;
		if (!appNetscape && contentIframe.onload)
			b = true;
		if (contentIframe.isloadforIE)
			b = true;
		return b;
	}
	
	var scrolling_tmp;
	var afterReSizeIframe;
	function reSizeIframe(n, maxHeight, isGrid, minHeight) {
		var pTop = document.body.scrollTop;
		var objif = findObj(n);
		var objifcall = eval(n);
		if (maxHeight == null || maxHeight == 0) { //有maxHeight值時要避免，例如舊式Gen樣板
			if (scrolling_tmp == null)
				scrolling_tmp = objif.getAttribute("scrolling");
			objif.setAttribute("scrolling", "no"); //為了修正硬要跑出來的SCROLLING
		}
		objif.height = 0; //配合objif.height = 0;，scroll到原高度
		var ifw = objif.contentWindow.document.body.scrollWidth;
		var ifh = objif.contentWindow.document.body.scrollHeight;
		//alert("scrollHeight:"+ifh);
		
		if (minHeight == null) {
			//alert("onload 1:"+objif.onload);
			minHeight = objif.height;
			//alert("minHeight:"+minHeight);
			objif.onload = function(event) {reSizeIframe(n, maxHeight, isGrid, minHeight);} //修正minHeight沒有設定進onload reSizeIframe
			//alert("onload 2:"+objif.onload);
			reSizeIframe(n, maxHeight, isGrid, minHeight);
			return;
		}
		
		if (isGrid) {
			ifh = objifcall.getGridHeight()+3;
			//alert("getGridHeight:"+ifh);
		}
		
		//alert("ifh:"+ifh+" objif:"+objif.clientHeight+" maxHeight:"+maxHeight);
		//alert("ifw:"+ifw+" objif:"+objif.clientWidth);
		if (ifw > objif.clientWidth) {
			objif.setAttribute("scrolling", scrolling_tmp); //超過範圍，回復原設定
			ifh = ifh+20;
		}
		else if (ifw+14 < objif.clientWidth) {
			//有maxHeight值時要避免，例如舊式Gen樣板，所以還是要有的，雙管齊下
			ifh = ifh+1; //為了修正硬要跑出來的SCROLLING
		}
		
		if (maxHeight != null && maxHeight != 0) {
			if (ifh < minHeight) {
				objif.height = minHeight;
			}	
			else if (ifh < maxHeight) {
				objif.height = ifh+Math.ceil(detectZoom()/100)-1;
			}		
			else {
				objif.height = maxHeight-((detectZoom()-100)/100*12);
			}
		}
		else {
			if (ifh < minHeight) {
				objif.height = minHeight;
			}	
			else {
				objif.height = ifh;
			}		
		}
		
		if (isGrid) {
			objifcall.showGrid();
		}
		
		document.body.scrollTop = pTop; //配合objif.height = 0;，scroll到原高度
		
		if (afterReSizeIframe) {
			eval(afterReSizeIframe);
			afterReSizeIframe = null;
		}
		
		
		if (objifcall.apoint) {
			var p = objifcall.apoint.lastIndexOf(".");
			if (p != -1) {
				var windowcall = eval(n+"."+objifcall.apoint.substring(0, p));
				windowcall.location.hash = objifcall.apoint.substring(p+1);
			}
			else {
				objifcall.location.hash = objifcall.apoint;
			}
			objifcall.apoint = null;
		}
	}	
	
	function getLeft(obj, srcObj) {
		var leftTemp = 0;
		if (obj.iHere == null)
			leftTemp += getParnetLeft(obj.parent, obj.name, obj);
			
		var position = srcObj.getBoundingClientRect();		
		leftTemp += position.left;
		//leftTemp += getOffsetLeft(srcObj, obj.iHere);
		
		if (srcObj.tagName == "TD")
			leftTemp += parseInt(srcObj.width) + 4;
		else if (srcObj.tagName == "INPUT")
			leftTemp += parseInt(srcObj.offsetWidth) + 2;
		else if (srcObj.tagName == "IMG")
			leftTemp += parseInt(srcObj.width) + 2;
		else	
			leftTemp += parseInt(srcObj.width);
		
		return leftTemp;
	}
	
	function getTop(obj, srcObj) {
		var topTemp = 0;
		if (obj.iHere == null)
			topTemp += getParnetTop(obj.parent, obj.name, obj);
			
		var position = srcObj.getBoundingClientRect();		
		topTemp += position.top;
		//topTemp += getOffsetTop(srcObj, obj.iHere);		
		
		return topTemp;
	}
	
	function getParnetLeft(obj, frameName, frame) {
		var leftTemp = 0;
		if (obj.iHere == null)
			leftTemp += getParnetLeft(obj.parent, obj.name, obj);
		
		leftTemp += parseInt(obj.document.all(frameName).frameBorder)*2 + getOffsetLeft(obj.document.all(frameName), obj.iHere);
					
		return leftTemp;		
	}
	
	function getParnetTop(obj, frameName, frame) {
		var topTemp = 0;
		if (obj.iHere == null)
			topTemp += getParnetTop(obj.parent, obj.name, obj);
	
		topTemp += parseInt(obj.document.all(frameName).frameBorder)*2 + getOffsetTop(obj.document.all(frameName), obj.iHere);

		return topTemp;		
	}
	
	function getOffsetLeft(obj, iH) {
		var leftTemp = 0;
		if (obj.offsetParent) 
			leftTemp += getOffsetLeft(obj.offsetParent, iH);
		
		if (obj.scrollLeft && iH == null)
			leftTemp -= obj.scrollLeft;
		
		if (obj.offsetLeft)
			leftTemp += obj.offsetLeft;
					
		return leftTemp;		
	}
	
	function getOffsetTop(obj, iH) {
		var topTemp = 0;
		if (obj.offsetParent)
			topTemp += getOffsetTop(obj.offsetParent, iH);
			
		if (obj.scrollTop && iH == null)
			topTemp -= obj.scrollTop;
		
		if (obj.offsetTop)
			topTemp += obj.offsetTop;
	
		return topTemp;		
	}	
	
	function pack(bytes) {
	    var chars = [];
	    for(var i = 0, n = bytes.length; i < n;) {
	        chars.push(((bytes[i++] & 0xff) << 8) | (bytes[i++] & 0xff));
	    }
	    return String.fromCharCode.apply(null, chars);
	}
	
	function unpack(str) {
	    var bytes = [];
	    for(var i = 0, n = str.length; i < n; i++) {
	        var char = str.charCodeAt(i);
	        bytes.push(char >>> 8, char & 0xFF);
	    }
	    return bytes;
	}
	
	function getStringWithRealLength(str, len) {
		var bytes = unpack(str);
		var j = 0;
		var k = 0;
	    for(var i=0; i<bytes.length; i=i+2) {
	    	if (bytes[i] > 0)
	    		j = j+2;
	    	else   		
	    		j = j+1;    		
	    		
	    	if (len != null && j > len)
	    		return str.substring(0, k);
	    		
	    	k++;
	    }
	    if (len != null)
	    	return str;
	    else	
	    	return j;
	}
	
	function getStringWithRealLengthLast(str, len) {		
		var st = "";
		while (str.length >= 1) {
			st += str.substring(str.length-1);
			str = str.substring(0, str.length-1);
		}
		st = getStringWithRealLength(st, len);
		
		str = st;
		st = "";
		while (str.length >= 1) {
			st += str.substring(str.length-1);
			str = str.substring(0, str.length-1);
		}
	  return st;
	}
	
	function getStringFixedShort(str, len) {		
		var i = getStringWithRealLength(str);
		if (i > len) {
			var s1 = getStringWithRealLength(str, (len-4)/2);
			var s2 = getStringWithRealLengthLast(str, (len-4)/2);		
			str = s1 + '...' + s2;
		}
	  return str;
	}

	function setCookie(name, value, expiry, path, domain, secure) {
		var NameString = name + "=" + escape(value);
		var ExpiryString = (expiry==null)?"":";expires=" + expiry.toGMTString();
		var PathString = (path==null)?"":";path=" + path;
		var DomainString = (path==null)?"":";domain=" + domain;
		var SecureString = (secure)?";secure":"";
		document.cookie = NameString + ExpiryString + PathString + DomainString + SecureString;
	}
	
	function getCookie(name) { 
		var arr = document.cookie.match(new RegExp("(^| )"+name+"=([^;]*)(;|$)")); 
		if (arr != null)
			return unescape(arr[2]);
		return null; 
	}
	
	function removeCookie(name) { 
		var exp = new Date(); 
		exp.setTime(exp.getTime() - 1); 
		var cval = getCookie(name); 
		if (cval != null)
			document.cookie= name + "="+cval+";expires="+exp.toGMTString(); 
	}

	var openModalDialog = null;
	var hasModalDialogSub = false;
	if (!window.showModalDialog) {
		window.showModalDialog = function(srcFile, selfObj, winFeatures) { //Object.prototype.showModalDialog = function(srcFile, selfObj, winFeatures) { // import Bootstrap error
			
			var iZoom = detectZoom();
			if (iZoom != 100) {
				var sB = "dialogHeight:";
				var sE = "px;";
				var iB = winFeatures.indexOf(sB);
				var iE;
				if (iB != -1) {
					iE = winFeatures.indexOf(sE, iB);
					winFeatures = winFeatures.substring(0, iB+sB.length) + parseInt(parseInt(winFeatures.substring(iB+sB.length, iE))*iZoom/100) + winFeatures.substring(iE);
				}
				sB = "dialogWidth:";
				iB = winFeatures.indexOf(sB);
				if (iB != -1) {
					iE = winFeatures.indexOf(sE, iB);
					winFeatures = winFeatures.substring(0, iB+sB.length) + parseInt(parseInt(winFeatures.substring(iB+sB.length, iE))*iZoom/100) + winFeatures.substring(iE);
				}
			}
			
			winFeatures = replaceAll(winFeatures, "dialogTop", "top");
			winFeatures = replaceAll(winFeatures, "dialogLeft", "left");
			winFeatures = replaceAll(winFeatures, "dialogHeight", "height");
			winFeatures = replaceAll(winFeatures, "dialogWidth", "width");
			winFeatures = replaceAll(winFeatures, "px", "");
			winFeatures = replaceAll(winFeatures, ":", "=");
			winFeatures = replaceAll(winFeatures, ";", ",");
			winFeatures = winFeatures + ", dialog=yes, modal=yes, directories=no";
			//alert("winFeatures="+winFeatures);			
				
			if (opener != null)
				opener.hasModalDialogSub = true;
			
			openModalDialog = window.open(srcFile, "openModalDialog"+new Date(), winFeatures);
			openModalDialog.onblur = closeOpenModalDialog;
			
			return openModalDialog;
		}
	}
	
	var checkWindowClose;
	if (opener != null) {
		checkWindowClose = self;
	}
	else if (parent != null && parent.opener != null) {
		checkWindowClose = parent;
	}	
	if (checkWindowClose != null) {
		if (checkWindowClose.onblur != null) {
			if (checkWindowClose.onblur == checkWindowClose.opener.closeOpenModalDialog) {
				window._alert = window.alert;
				window.alert = function(str) {
				     //if(console) console.log(str);
				     checkWindowClose.onblur = null;
				     window._alert(str);
				     setTimeout("checkWindowClose.onblur = checkWindowClose.opener.closeOpenModalDialog;", 100);
				}
	
				window._confirm = window.confirm;
				window.confirm = function(str) {
				     //if(console) console.log(str);
				     checkWindowClose.onblur = null;
				     var r = window._confirm(str);
				     setTimeout("checkWindowClose.onblur = checkWindowClose.opener.closeOpenModalDialog;", 100);
				     return r;
				}
	
				window._prompt = window.prompt;
				window.prompt = function(str, def) {
				     //if(console) console.log(str+", "+def);
				     checkWindowClose.onblur = null;
				     var r = window._prompt(str, def);
				     setTimeout("checkWindowClose.onblur = checkWindowClose.opener.closeOpenModalDialog;", 100);
				     return r;
				}
			}
		}
	}
	
	function closeOpenModalDialog() {
		if (!hasModalDialogSub && !openModalDialog.document.hasFocus()) {
			openModalDialog.close();
			openModalDialog = null;
		}		
		
		if (opener != null && opener.hasModalDialogSub) {
			opener.focus();
			opener.hasModalDialogSub = false;
			opener.setTimeout(function() {
					if(!document.hasFocus())
						self.close();
				}, 100);
		}
	}
	
	function setWinFeaturesPosition(winFeatures) {
		
		var iHeightStart = winFeatures.indexOf("dialogHeight");
		iHeightStart = winFeatures.indexOf(":", iHeightStart+1);
		var iHeightEnd = winFeatures.indexOf("px", iHeightStart+1);
		var iWidthStart = winFeatures.indexOf("dialogWidth");
		iWidthStart = winFeatures.indexOf(":", iWidthStart+1);
		var iWidthEnd = winFeatures.indexOf("px", iWidthStart+1);
		
		//指定子視窗位置
		var dialogHeight = parseInt(winFeatures.substring(iHeightStart+1, iHeightEnd));
		var dialogWidth = parseInt(winFeatures.substring(iWidthStart+1, iWidthEnd));
		//alert("dialogHeight="+dialogHeight+", dialogWidth="+dialogWidth);
		
		var centerPosition = getCenterPosition(dialogHeight, dialogWidth);
		if (centerPosition != null) {
			winFeatures = "dialogTop:"+centerPosition[0]+"px; dialogLeft:"+centerPosition[1]+"px; "+winFeatures;
		}
		
		return winFeatures;
	}
	
	function getCenterPosition(dialogHeight, dialogWidth, alwaysCalc) {		
		//alert(window.screenX);
		//alert(screen.availHeight);
		//alert(top.document.body.scrollHeight);
		//alert(screen.availHeight-top.document.body.scrollHeight);
		
		//alert(window.screenLeft);
		//alert(window.screenX);
		//alert(screen.width);
		//alert(screen.availWidth);
		//alert(parent.document.body.scrollWidth);
		//alert(top.document.body.scrollWidth);
		//alert(screen.availWidth-parent.document.body.scrollWidth);

		//var dialogTop = (top.document.body.scrollHeight/2) - (dialogHeight/2) + (screen.availHeight-top.document.body.scrollHeight);
		//var dialogLeft = (parent.document.body.scrollWidth/2) - (dialogWidth/2) + (screen.availWidth-parent.document.body.scrollWidth);
		
		// Fixes dual-screen position
		var dualScreenLeft = window.screenLeft != undefined ? window.screenLeft : screen.left;
		if (opener) {
			dualScreenLeft = opener.screenLeft != undefined ? opener.screenLeft : screen.left;
		}
			
		if (window.screenX != null) {
			dualScreenLeft = window.screenX;
			if (opener) {
				dualScreenLeft = opener.screenX;
			}
		}
		//alert("dualScreenLeft="+dualScreenLeft);
    
		if (alwaysCalc || window.screenX != null) {
			
			var iZoom = detectZoom();
			if (iZoom != 100) {
				dialogHeight = parseInt(parseInt(dialogHeight)*iZoom/100);
				dialogWidth = parseInt(parseInt(dialogWidth)*iZoom/100);
			}

			var dialogTop = (screen.height/2) - ((dialogHeight+46)/2);
			var dialogLeft = (screen.width/2) - (dialogWidth/2) + dualScreenLeft;
			//alert("dialogTop="+dialogTop+", dialogLeft="+dialogLeft);
			
			return new Array (dialogTop, dialogLeft);
		}
		
		return null;
	}

	function goHref(sHref) {
		location = sHref;
	}
	
	function detectZoom() { 
		var ratio = 0,
		screen = window.screen,
		ua = navigator.userAgent.toLowerCase();
		
		if (window.outerWidth !== undefined && top.innerWidth !== undefined) {
			ratio = window.outerWidth / top.innerWidth;
		}
		else if (window.devicePixelRatio !== undefined) {
			ratio = window.devicePixelRatio;
		}
		else if (~ua.indexOf('msie')) {
			if (screen.deviceXDPI && screen.logicalXDPI) {
				ratio = screen.deviceXDPI / screen.logicalXDPI;
			}
		}
		
		if (ratio) {
			ratio = Math.round(ratio * 100);
		}
		
		return ratio;
	};