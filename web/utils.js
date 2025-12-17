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

export function deleteCookie(name) {
   document.cookie = `${name}=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;`;
}

export function test() {
   console.log("test");
}