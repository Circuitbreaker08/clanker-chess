export function getCookieByName(name) {
    const cookies = document.cookie.split(';');
    for (let cookie of cookies) {
         cookie = cookie.trim();
         if (cookie.startsWith(name + '=')) {
            return cookie.substring(name.length + 1);
         }
    }
   return null;
}

export function test() {
   console.log("test");
}