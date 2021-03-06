_loadScript('lhcb.display.items.cpp');
_loadScript('lhcb.display.listener.cpp');
_loadFile('lhcb.display.general','css');
_loadFile('lhcb.display.status','css');
_loadFile('lhcb.display.fsm','css');


FSMTable = function(partition,msg,state) {
  table           = document.createElement('table');
  table.body      = document.createElement('tbody');
  table.provider  = null;
  table.listener  = null;
  table.logger    = null;

  table._system   = partition;
  table._messages = msg;
  table._state    = state;
  
  /**
   */
  table.add = function(object) {
    tr = document.createElement('tr');
    td = document.createElement('td');
    td.appendChild(object);
    td.colSpan = 3;
    tr.appendChild(td);
    this.body.appendChild(tr);
  };

  /**
   */
  table.close_display = function() {
    var self = this.handler;
    self.logger.info('close_display: '+self.listener+' self.system:'+self._system);
    if ( self.listener )  {
      self.listener.close_child(self._system);
    }
  };

  /**
   */
  table.createDisplay = function()   {
    if ( null != this.listener ) this.listener.close2();
    this.listener = new FSMListener(this.logger,this.provider,this.display,this._messages);
    this.listener.trace = false;
    this.listener.start(this._system,'lbWeb.'+this._system+'.FSM.children');
  };

  table.className = 'FSMStatus';
  table.body.className = 'FSMStatus';

  table.display = document.createElement('div');
  table.logDisplay = document.createElement('div');

  var tr = document.createElement('tr');
  var td = document.createElement('td');
  td.innerHTML = table._system;
  if ( table._state )  {
    td.innerHTML = td.innerHTML+' is '+table._state;
    td.innerHTML = td.innerHTML+' <IMG SRC="'+_fileBase+'/Images/Modes/'+table._state+'.bmp">';
  }
  td.className = 'StatusPlain';
  tr.appendChild(td);
  /*
  table.update = document.createElement('td');
  table.update.cellPadding = 0;
  table.update.className   = 'DisplayButton';
  table.update.innerHTML   = 'Update';
  table.update.onclick     = function()   { _dataProvider.update(); };
  table.update.handler     = table;
  table.update.visible     = false;
  tr.appendChild(table.update);
  */

  table.close = document.createElement('td');
  table.close.className    = 'DisplayButton';
  table.close.innerHTML    = 'Close';
  table.close.cellPadding  = 0;
  table.close.onclick      = table.close_display;
  table.close.handler      = table;
  tr.appendChild(table.close);

  table.body.appendChild(tr);

  table.add(table.display);
  table.add(table.logDisplay);

  table.appendChild(table.body);
  return table;
};

fsm_unload = function()  {
  dataProviderReset();
  //alert('Connection reset.\n\nBye, Bye my friend....');
};

fsm_body2 = function(body)  {
  var sys   = the_displayObject['system'];
  var msg   = the_displayObject['messages'];
  var state = the_displayObject['state'];

  //alert('1 system:'+sys+' messages:'+msg+' state:'+state);
  var tips  = init_tooltips(body);
  var selector = FSMTable(sys,msg,state);
  body.appendChild(selector);
  body.className = 'MainBody';
  if ( msg == null ) {
    selector.logger = new OutputLogger(selector.logDisplay,  -1, LOG_INFO,'StatusLogger');
  }
  else {
    selector.logger = new OutputLogger(selector.logDisplay, 200, LOG_INFO,'StatusLogger');
  }
  selector.provider = new DataProvider(selector.logger);
  selector.provider.topic = '/topic/status';
  selector.createDisplay();
  selector.provider.start();
};

fsm_body = function()  {
  var body = document.getElementsByTagName('body')[0];
  fsm_body2(body);
};

if ( _debugLoading ) alert('Script lhcb.display.fsm.cpp loaded successfully');
