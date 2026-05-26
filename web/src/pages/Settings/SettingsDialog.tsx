import { Button } from "@/components/ui/button"
import {
  Dialog,
  DialogClose,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog"
import { Slider } from "@/components/ui/slider"
import { Input } from "@/components/ui/input"
import { SettingsIcon } from "lucide-react"
import NeopixelSettings from "./Modules/NeopixelSettings"

import ModuleCard from "./ModuleCard"
import { Progress } from "@/components/ui/progress"


export default function SettingsDialog() {

  return (
    <Dialog>
        <DialogTrigger><Button variant="secondary" size="icon" className="">
                    <SettingsIcon/>
                </Button></DialogTrigger>
        <DialogContent>


            <DialogHeader>
                <DialogTitle>"Left Arch" Controller Settings</DialogTitle>
                <DialogDescription>
                    This modifies the actual controller settings, not the project settings.
                </DialogDescription>
            </DialogHeader>


            <p className="text-md font-semibold -mb-2 mt-2 flex justify-between">SD Card Storage
                <span className="font-bold">
                    12.5<span className="text-muted-foreground text-xs font-semibold">/32 GB</span>
                </span>
            </p>
            <Progress className="" value={40} />



            <p className="text-md font-semibold -mb-4 mt-2">Name</p>
            <p className="text-md font-semibold text-muted-foreground -mb-2">
                Display name for the controller.
            </p>
            <Input placeholder="Left Arch" />

            
            <p className="text-md font-semibold -mb-4 mt-2">Volume</p>
            <p className="text-md font-semibold text-muted-foreground -mb-2">
                Adjust the volume of the controller's DAC.
            </p>
            <Slider className="mt-1" defaultValue={[33]} max={100} step={1} />

            <p className="text-md font-semibold -mb-4 mt-2">Module Configuration</p>
            <p className="text-md font-semibold text-muted-foreground -mb-2">
                Select the module layout on the controller.
            </p>
            <section className="grid gap-2 grid-cols-4">
                <NeopixelSettings/>
                <ModuleCard name="Empty"/>
                <ModuleCard name="Empty"/>
                <ModuleCard name="Empty"/>
            </section>



           



            <DialogFooter>
                <DialogClose asChild>
                    <Button variant="outline">Cancel</Button>
                </DialogClose>
                <Button type="submit">Save changes</Button>
            </DialogFooter>



        </DialogContent>
    </Dialog>
  )
}