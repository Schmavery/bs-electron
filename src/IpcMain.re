module type IpcType = {
  type rendererToMainMessages;
  let message: string;
};

module MakeIpcMain = (T: IpcType) => {
  type event = {. "sender": {. [@bs.meth] "send": (string, string) => unit}};
  type messageCallback('a) = (. event, 'a) => unit;
  type ipcCallback = (. event, string) => unit;
  [@bs.module "electron"] [@bs.scope "ipcMain"]
  external on: (string, ipcCallback) => unit = "";
  let on = (cb: messageCallback(T.rendererToMainMessages)) =>
    on(T.message, (. event, arg) =>
      cb(. event, arg->Js.Json.parseExn->Json.fromValidJson)
    );
  [@bs.module "electron"] [@bs.scope "ipcMain"]
  external removeListener:
    (string, messageCallback(T.rendererToMainMessages)) => unit =
    "";
  let removeListener = removeListener(T.message);
};
