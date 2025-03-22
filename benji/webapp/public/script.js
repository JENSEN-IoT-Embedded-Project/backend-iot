document.addEventListener("DOMContentLoaded", () => {
    document.querySelectorAll(".box").forEach(box => {
        box.addEventListener("click", () => {
            const route = box.innerText.toLowerCase().replace(/\s+/g, "_");
            window.location.href = `sub_pages/${route}`;
        });
    });
});
